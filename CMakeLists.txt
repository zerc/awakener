cmake_minimum_required(VERSION 3.22)
project(awakener C)

set(CMAKE_C_STANDARD 17)

add_executable(awakener server.h server.c main.c)
