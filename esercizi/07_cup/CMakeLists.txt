cmake_minimum_required(VERSION 3.23)
project(07_cup C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

include_directories(.)

add_executable(client client.c
                      socket_utility.c socket_utility.h
                      application_level_utility.c application_level_utility.h)

add_executable(server server.c
                      socket_utility.c socket_utility.h
                      signal_handler.c  signal_handler.h
                      application_level_utility.c application_level_utility.h
                      bool_utility.c bool_utility.h)