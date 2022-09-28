cmake_minimum_required(VERSION 3.23)
project(03_client_counter C)

set(CMAKE_C_STANDARD 99)

add_executable(client client.c socket_utility.c)
add_executable(server server.c socket_utility.c signal_handler.c)
