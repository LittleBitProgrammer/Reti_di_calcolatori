cmake_minimum_required(VERSION 3.23)
project(01_wrapper C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(01_wrapper
        client.c
        server.c
        socket_utility.c
        socket_utility.h)
