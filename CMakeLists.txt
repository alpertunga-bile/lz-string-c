cmake_minimum_required(VERSION 3.5.0)
project(lz-string-c VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

set(CMAKE_BUILD_TYPE "Debug")

option(PXD_BUILD_TEST "Build test executable" ON)

set(PROJECT_C_STANDARD 99)

if(NOT WIN32)
    set(IWYU_ARGS "include-what-you-use -w -Xiwyu --verbose=7")
    add_compile_definitions(CMAKE_C_INCLUDE_WHAT_YOU_USE=${IWYU_ARGS})
endif(NOT WIN32)

set(PXD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(PXD_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(PXD_HEADER_FILES
    ${PXD_INCLUDE_DIR}/lz_string.h
    ${PXD_INCLUDE_DIR}/hash_map.h
    ${PXD_INCLUDE_DIR}/char_dynamic_array.h
    ${PXD_INCLUDE_DIR}/string_dynamic_array.h
    ${PXD_INCLUDE_DIR}/c_string.h
)

set(PXD_SOURCE_FILES
    ${PXD_INCLUDE_DIR}/lz_string.c
    ${PXD_INCLUDE_DIR}/hash_map.c
    ${PXD_INCLUDE_DIR}/char_dynamic_array.c
    ${PXD_INCLUDE_DIR}/string_dynamic_array.c
    ${PXD_INCLUDE_DIR}/c_string.c
)

include_directories(
    ${PXD_INCLUDE_DIR}
)

set(COMMON_STD_HEADERS
    <string.h>
    <stdint.h>
    <stddef.h>
    <stdlib.h>
)

set(LIBS_TO_LINK
)

add_library(${PROJECT_NAME} ${PXD_SOURCE_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES C_STANDARD ${PROJECT_C_STANDARD})

target_link_libraries(${PROJECT_NAME} ${LIBS_TO_LINK})

target_precompile_headers(
    ${PROJECT_NAME} PRIVATE
    ${COMMON_STD_HEADERS}
    ${PXD_HEADER_FILES}
)

if(PXD_BUILD_TEST)
    set(PXD_TEST_PROJECT_NAME lz-string-c-test)
    set(PXD_TEST_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test_src)

    set(PXD_TEST_SOURCE_FILES
        ${PXD_TEST_SRC_DIR}/test.c

        ${PXD_TEST_SRC_DIR}/utility.h
        ${PXD_TEST_SRC_DIR}/utility.c

        ${PXD_TEST_SRC_DIR}/rktest.h

        ${PXD_SOURCE_FILES}
    )

    add_executable(${PXD_TEST_PROJECT_NAME} ${PXD_TEST_SOURCE_FILES})

    set_target_properties(${PXD_TEST_PROJECT_NAME} PROPERTIES C_STANDARD ${PROJECT_C_STANDARD})

    if(NOT WIN32)
      target_link_libraries(${PXD_TEST_PROJECT_NAME} ${LIBS_TO_LINK} m)
    else()
      target_link_libraries(${PXD_TEST_PROJECT_NAME} ${LIBS_TO_LINK})
    endif(NOT WIN32)

    target_precompile_headers(
        ${PXD_TEST_PROJECT_NAME} PRIVATE
        ${PXD_TEST_SRC_DIR}/utility.h
        ${COMMON_STD_HEADERS}
    )

endif(PXD_BUILD_TEST)
unset(PXD_BUILD_TEST CACHE)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
