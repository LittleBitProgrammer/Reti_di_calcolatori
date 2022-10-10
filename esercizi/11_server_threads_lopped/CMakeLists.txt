cmake_minimum_required(VERSION 3.23)
project(11_server_threads_lopped C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

include_directories(.)

add_executable(
        client
        client.c
        socket_utility.c
        socket_utility.h)

add_executable(
        server
        server.c
        bool_utility.c
        bool_utility.h
        signal_handler.c
        signal_handler.h
        socket_utility.c
        socket_utility.h thread_utility.c thread_utility.h)
