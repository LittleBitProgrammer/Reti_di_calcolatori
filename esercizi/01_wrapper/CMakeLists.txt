cmake_minimum_required(VERSION 3.23)
project(01_wrapper C)

set(CMAKE_C_STANDARD 11)
SET(CMAKE_BUILD_TYPE Debug)

include_directories(.)

add_executable(server
        server.c
        socket_utility.c)

add_executable(client
        client.c
        socket_utility.c)
