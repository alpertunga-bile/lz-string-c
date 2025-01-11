#include "lz_string.h"

#include <stdint.h>
#include <stddef.h>
#include <math.h>

#include "char_dynamic_array.h"
#include "hash_map.h"
#include "string_dynamic_array.h"

/*
 * --------------------------------------------------------------------------------------
 * -- Function forward declarations
 */

void
lz_compress(string_t* uncompressed,
            uint8_t   bits_per_char,
            char (*get_char_from_int)(uint8_t),
            string_t* compressed);

void
lz_decompress(string_t*   compressed,
              uint32_t    reset_value,
              hash_map_t* alphabet,
              uint8_t (*get_next_value)(hash_map_t*, char),
              string_t* uncompressed);

const char key_str_64[66] =
  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

char
get_char_from_int_base64(uint8_t value)
{
  return key_str_64[value];
}

uint8_t
get_next_value_base64(hash_map_t* alphabet, char character)
{
  uint32_t value = 0;
  hash_map_get_value(alphabet, character, &value);

  return value;
}

/*
 * --------------------------------------------------------------------------------------
 * -- Function implementations
 */

void
lz_compress_base64(string_t* input, string_t* compressed)
{
  if (input->length == 0) {
    string_free(compressed);
    return;
  }

  lz_compress(input, 6, get_char_from_int_base64, compressed);

  if (compressed->length == 0) {
    return;
  }

  switch (compressed->length % 4) {
    case 0:
      break;
    case 1:
      string_append_pointer(compressed, "===");
      break;
    case 2:
      string_append_pointer(compressed, "==");
      break;
    case 3:
      string_append_char(compressed, '=');
      break;
    default:
      break;
  }
}

void
lz_decompress_base64(string_t* compressed, string_t* uncompressed)
{
  if (compressed->length == 0) {
    string_free(uncompressed);
    return;
  }

  hash_map_t dictionary;
  hash_map_init(&dictionary);

  int i = 0;

  for (i = 0; i < 66; ++i) {
    hash_map_add_value(&dictionary, key_str_64[i], i);
  }

  lz_decompress(
    compressed, 32, &dictionary, get_next_value_base64, uncompressed);

  hash_map_free(&dictionary);
}

void
lz_compress(string_t* uncompressed,
            uint8_t   bits_per_char,
            char (*get_char_from_int)(uint8_t),
            string_t* compressed)
{
  if (0 == uncompressed->length) {
    return;
  }

  uint32_t value = 0;

  hash_map_t context_dictionary;
  hash_map_t context_dictionary_to_create;
  hash_map_init(&context_dictionary);
  hash_map_init(&context_dictionary_to_create);

  string_t context_c;
  string_t context_wc;
  string_t context_w;
  string_t temp_string;

  string_init(&context_c);
  string_init(&context_wc);
  string_init(&context_w);
  string_init(&temp_string);

  int      context_enlarge_in = 2;
  uint32_t context_dict_size  = 3;
  uint32_t context_numbits    = 2;

  char_dynamic_array_t context_data;
  init_char_da(&context_data, 5, 5);

  uint32_t context_data_val      = 0;
  uint32_t context_data_position = 0;

  int ii = 0;
  int i  = 0;

  const size_t uncompressed_length = uncompressed->length;

  for (ii = 0; ii < uncompressed_length; ++ii) {
    string_set_char(&context_c, string_char_at(uncompressed, ii));

    if (-1 ==
        hash_map_contains_key(&context_dictionary, string_hash(&context_c))) {
      hash_map_add_value(
        &context_dictionary, string_hash(&context_c), context_dict_size++);
      hash_map_add_value(
        &context_dictionary_to_create, string_hash(&context_c), 1);
    }

    string_set_string(&temp_string, &context_w);
    string_append_string(&temp_string, &context_c);
    string_set_string(&context_wc, &temp_string);

    if (0 ==
        hash_map_contains_key(&context_dictionary, string_hash(&context_wc))) {
      string_set_string(&context_w, &context_wc);
    } else {
      if (0 == hash_map_contains_key(&context_dictionary_to_create,
                                     string_hash(&context_w))) {
        for (i = 0; i < context_numbits; ++i) {
          context_data_val = context_data_val << 1u;

          if (context_data_position != bits_per_char - 1u) {
            context_data_position++;
            continue;
          }

          context_data_position = 0;
          add_char_da(&context_data, get_char_from_int(context_data_val));
          context_data_val = 0;
        }

        value = string_char_at(&context_w, 0);

        for (i = 0; i < 8; ++i) {
          context_data_val = (context_data_val << 1u) | (value & 1u);
          value            = value >> 1u;

          if (context_data_position != bits_per_char - 1) {
            context_data_position++;
            continue;
          }

          context_data_position = 0;
          add_char_da(&context_data, get_char_from_int(context_data_val));
          context_data_val = 0;
        }

        context_enlarge_in--;
        if (0 == context_enlarge_in) {
          context_enlarge_in = pow(2, context_numbits);
          context_numbits++;
        }

        hash_map_remove_value(&context_dictionary_to_create,
                              string_hash(&context_w));
      } else {
        hash_map_get_value(
          &context_dictionary, string_hash(&context_w), &value);

        for (i = 0; i < context_numbits; ++i) {
          context_data_val = (context_data_val << 1u) | (value & 1u);
          value            = value >> 1u;

          if (context_data_position != bits_per_char - 1) {
            context_data_position++;
            continue;
          }

          context_data_position = 0;
          add_char_da(&context_data, get_char_from_int(context_data_val));
          context_data_val = 0;
        }
      }

      context_enlarge_in--;
      if (0 == context_enlarge_in) {
        context_enlarge_in = pow(2, context_numbits);
        context_numbits++;
      }

      hash_map_add_value(
        &context_dictionary, string_hash(&context_wc), context_dict_size++);
      string_set_string(&context_w, &context_c);
    }
  }

  if (context_w.length != 0) {
    if (0 == hash_map_contains_key(&context_dictionary_to_create,
                                   string_hash(&context_w))) {
      for (i = 0; i < context_numbits; ++i) {
        context_data_val = context_data_val << 1u;

        if (context_data_position != bits_per_char - 1) {
          context_data_position++;
          continue;
        }

        context_data_position = 0;
        add_char_da(&context_data, get_char_from_int(context_data_val));
        context_data_val = 0;
      }

      value = string_char_at(&context_w, 0);

      for (i = 0; i < 8; ++i) {
        context_data_val = (context_data_val << 1u) | (value & 1u);
        value            = value >> 1u;

        if (context_data_position != bits_per_char - 1) {
          context_data_position++;
          continue;
        }

        context_data_position = 0;
        add_char_da(&context_data, get_char_from_int(context_data_val));
        context_data_val = 0;
      }

      context_enlarge_in--;
      if (context_enlarge_in == 0) {
        context_enlarge_in = pow(2, context_numbits);
        context_numbits++;
      }

      hash_map_remove_value(&context_dictionary_to_create,
                            string_hash(&context_w));
    } else {
      hash_map_get_value(&context_dictionary, string_hash(&context_w), &value);

      for (i = 0; i < context_numbits; ++i) {
        context_data_val = (context_data_val << 1u) | (value & 1u);
        value            = value >> 1u;

        if (context_data_position != bits_per_char - 1) {
          context_data_position++;
          continue;
        }

        context_data_position = 0;
        add_char_da(&context_data, get_char_from_int(context_data_val));
        context_data_val = 0;
      }
    }

    context_enlarge_in--;
    if (context_enlarge_in == 0) {
      context_enlarge_in = pow(2, context_numbits);
      context_numbits++;
    }
  }

  value = 2;
  for (i = 0; i < context_numbits; ++i) {
    context_data_val = (context_data_val << 1u) | (value & 1u);
    value            = value >> 1u;

    if (context_data_position != bits_per_char - 1) {
      context_data_position++;
      continue;
    }

    context_data_position = 0;
    add_char_da(&context_data, get_char_from_int(context_data_val));
    context_data_val = 0;
  }

  int loop = 1;

  do {
    context_data_val = context_data_val << 1u;

    if(context_data_position != bits_per_char - 1) {
      context_data_position++;
    }

    add_char_da(&context_data, get_char_from_int(context_data_val));
    loop = 0;
  } while (loop == 1);

  add_char_da(&context_data, '\0');

  string_set_pointer(compressed, context_data.array);

  hash_map_free(&context_dictionary);
  hash_map_free(&context_dictionary_to_create);

  free_char_da(&context_data);

  string_free(&context_c);
  string_free(&context_wc);
  string_free(&context_w);
  string_free(&temp_string);
}

typedef struct
{
  uint32_t value;
  uint32_t position;
  uint32_t index;
} decompression_tracker_t;

void
lz_decompress(string_t*   compressed,
              uint32_t    reset_value,
              hash_map_t* alphabet,
              uint8_t (*get_next_value)(hash_map_t*, char),
              string_t* uncompressed)
{
  string_dynamic_array_t dictionary;
  da_string_init(&dictionary, 10, 5);

  string_dynamic_array_t result;
  da_string_init(&result, 10, 5);

  decompression_tracker_t data;
  data.value    = get_next_value(alphabet, string_char_at(compressed, 0));
  data.position = reset_value;
  data.index    = 1;

  uint32_t enlarge_in = 4;
  uint32_t dict_size  = 4;
  uint32_t numbits    = 3;

  string_t entry;
  string_init(&entry);

  char   character        = '\0';
  size_t character_number = 0;

  uint32_t bits      = 0;
  uint32_t max_power = pow(2, 2);
  uint32_t power     = 1;

  int i = 0;

  da_string_add_char(&dictionary, '0');
  da_string_add_char(&dictionary, '1');
  da_string_add_char(&dictionary, '2');

  while (power != max_power) {
    uint32_t resb = data.value & data.position;

    data.position >>= 1;

    if (data.position == 0) {
      data.position = reset_value;
      data.value =
        get_next_value(alphabet, string_char_at(compressed, data.index++));
    }

    bits   |= (resb > 0 ? 1 : 0) * power;
    power <<= 1;
  }

  switch (bits) {
    case 0:
      bits      = 0;
      max_power = pow(2, 8);
      power     = 1;

      while (power != max_power) {
        uint32_t resb = data.value & data.position;

        data.position >>= 1;

        if (data.position == 0) {
          data.position = reset_value;
          data.value =
            get_next_value(alphabet, string_char_at(compressed, data.index++));
        }

        bits   |= (resb > 0 ? 1 : 0) * power;
        power <<= 1;
      }
      character = bits;
      break;
    case 1:
      bits      = 0;
      max_power = pow(2, 16);
      power     = 1;

      while (power != max_power) {
        uint32_t resb = data.value & data.position;

        data.position >>= 1;

        if (data.position == 0) {
          data.position = reset_value;
          data.value =
            get_next_value(alphabet, string_char_at(compressed, data.index++));
        }

        bits   |= (resb > 0 ? 1 : 0) * power;
        power <<= 1;
      }
      character = bits;
      break;
    case 2:
      return;
  }

  if (character == 0) {
    return;
  }

  da_string_add_char(&dictionary, character);

  string_t w;
  string_init(&w);
  string_set_char(&w, character);

  da_string_add_char(&result, character);
  int forever = 1;

  while (forever == 1) {
    if (data.index > compressed->length) {
      string_set_char(uncompressed, '\0');

      string_free(&entry);
      string_free(&w);

      da_string_free(&dictionary);
      da_string_free(&result);

      return;
    }

    bits      = 0;
    max_power = pow(2, numbits);
    power     = 1;

    while (power != max_power) {
      uint32_t resb = data.value & data.position;

      data.position >>= 1;

      if (data.position == 0) {
        data.position = reset_value;
        data.value =
          get_next_value(alphabet, string_char_at(compressed, data.index++));
      }

      bits   |= (resb > 0 ? 1 : 0) * power;
      power <<= 1;
    }
    character_number = bits;

    switch (bits) {
      case 0:
        bits      = 0;
        max_power = pow(2, 8);
        power     = 1;

        while (power != max_power) {
          uint32_t resb = data.value & data.position;

          data.position >>= 1;

          if (data.position == 0) {
            data.position = reset_value;
            data.value    = get_next_value(
              alphabet, string_char_at(compressed, data.index++));
          }

          bits   |= (resb > 0 ? 1 : 0) * power;
          power <<= 1;
        }

        da_string_add_char(&dictionary, bits);
        character_number = dict_size;
        dict_size++;
        enlarge_in--;
        break;
      case 1:
        bits      = 0;
        max_power = pow(2, 16);
        power     = 1;

        while (power != max_power) {
          uint32_t resb = data.value & data.position;

          data.position >>= 1;

          if (data.position == 0) {
            data.position = reset_value;
            data.value    = get_next_value(
              alphabet, string_char_at(compressed, data.index++));
          }

          bits   |= (resb > 0 ? 1 : 0) * power;
          power <<= 1;
        }

        da_string_add_char(&dictionary, bits);
        character_number = dict_size;
        dict_size++;
        enlarge_in--;
        break;
      case 2:
        join_string_array(uncompressed, result.array, result.element_size);

        string_free(&entry);
        string_free(&w);

        da_string_free(&dictionary);
        da_string_free(&result);

        return;
    }

    if (enlarge_in == 0) {
      enlarge_in = pow(2, numbits);
      numbits++;
    }

    string_append_char(&w, string_char_at(&w, 0));

    if (dictionary.element_size > character_number) {
      string_set_string(&entry, &dictionary.array[character_number]);
    } else {
      if (character_number == dict_size) {
        string_append_string(&entry, &w);
      } else {
        string_free(&entry);
        string_free(&w);

        da_string_free(&dictionary);
        da_string_free(&result);

        return;
      }
    }

    da_string_add_string(&result, &entry);
    da_string_add_string(&dictionary, &w);
    dict_size++;
    enlarge_in--;
    string_set_string(&w, &entry);

    if (enlarge_in == 0) {
      enlarge_in = pow(2, numbits);
      numbits++;
    }
  }

  string_free(&entry);
  string_free(&w);

  da_string_free(&dictionary);
  da_string_free(&result);
}
