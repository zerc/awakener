cmake_minimum_required(VERSION 3.22)
project(awakener C)

set(CMAKE_C_STANDARD 17)

add_executable(awakener server.h server.c main.c)

add_custom_target(copy-templates ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/public ${CMAKE_BINARY_DIR}/public
)