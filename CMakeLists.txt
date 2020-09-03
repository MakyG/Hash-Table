cmake_minimum_required(VERSION 2.8)

project(Hash_table C)

#aux_source_directory( . SRC )
file(GLOB sources "src/*.c")
file(GLOB headers "src/*.h")
list(REMOVE_ITEM sources ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c)

file(GLOB testSources "tests/*.c")
#aux_source_directory( tests TESTS)

add_compile_options(-Wall -Wextra -std=c99 -Werror)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(${PROJECT_NAME} ${sources} ${headers} src/main.c)
add_executable(tests ${sources} ${headers} ${testSources})
if (UNIX)
        target_compile_definitions(tests PRIVATE _POSIX_C_SOURCE=199309L)
endif (UNIX)
