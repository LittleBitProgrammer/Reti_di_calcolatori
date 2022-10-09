cmake_minimum_required(VERSION 3.23)
project(09_client_server_multiplex_v2 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_BUILD_TYPE Debug)

include_directories(.)

add_executable(client client.c socket_utility.c)
add_executable(server server.c socket_utility.c signal_handler.c number_utility.c bool_utility.h)
