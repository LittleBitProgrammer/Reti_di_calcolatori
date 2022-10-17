cmake_minimum_required(VERSION 3.23)
project(Codice C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/output)

include_directories(.)

# ==========================
# =      Eseguibili        =
# ==========================
# Aggiungiamo all'elenco dei file eseguibili singolarmente il:
# - client vaccinato
add_executable(client_vaccinato vaccinated_client.c
                                lib/sockets_utility.c lib/sockets_utility.h
                                lib/menu_utility.c lib/menu_utility.h
                                lib/date_utility.c lib/date_utility.h
                                lib/bool_utility.h lib/bool_utility.c
                                lib/code_verification.c lib/code_verification.h
                                lib/digits_utility.c lib/digits_utility.h
                                lib/file_utility.c lib/file_utility.h
                                lib/buffer_utility.c lib/buffer_utility.h
                                lib/package_utility.c lib/package_utility.h
                                lib/green_pass_utility.c lib/green_pass_utility.h)

# - client amministratore
add_executable(client_amministratore administrator_client.c
                                     lib/sockets_utility.c lib/sockets_utility.h
                                     lib/menu_utility.c lib/menu_utility.h
                                     lib/package_utility.c lib/package_utility.h
                                     lib/date_utility.c lib/date_utility.h
                                     lib/code_verification.c lib/code_verification.h
                                     lib/buffer_utility.c lib/buffer_utility.h
                                     lib/digits_utility.c lib/digits_utility.h
                                     lib/file_utility.c lib/file_utility.h
                                     lib/green_pass_utility.c lib/green_pass_utility.h)

# - client revisore
add_executable(client_revisore reviser_client.c
                               lib/sockets_utility.c lib/sockets_utility.h
                               lib/menu_utility.c lib/menu_utility.h
                               lib/date_utility.c lib/date_utility.h
                               lib/bool_utility.h lib/bool_utility.c
                               lib/code_verification.c lib/code_verification.h
                               lib/digits_utility.c lib/digits_utility.h
                               lib/file_utility.c lib/file_utility.h
                               lib/buffer_utility.c lib/buffer_utility.h
                               lib/package_utility.c lib/package_utility.h
                               lib/green_pass_utility.c lib/green_pass_utility.h)

# - server centro vaccinale
add_executable(server_centro_vaccinale vaccination_center_server.c
                                       lib/thread_utility.c lib/thread_utility.h
                                       lib/sockets_utility.c lib/sockets_utility.h
                                       lib/bool_utility.h lib/bool_utility.c
                                       lib/file_utility.c lib/file_utility.h
                                       lib/date_utility.c lib/date_utility.h
                                       lib/green_pass_utility.c lib/green_pass_utility.h)

# - server centrale
add_executable(server_centrale central_server.c
                               lib/sockets_utility.c lib/sockets_utility.h
                               lib/thread_utility.c lib/thread_utility.h
                               lib/bool_utility.h lib/bool_utility.c
                               lib/file_utility.c lib/file_utility.h
                               lib/date_utility.c lib/date_utility.h
                               lib/green_pass_utility.c lib/green_pass_utility.h)

# - server assistente
add_executable(server_assistente assistant_server.c
                                 lib/sockets_utility.c lib/sockets_utility.h
                                 lib/thread_utility.c lib/thread_utility.h
                                 lib/bool_utility.h lib/bool_utility.c
                                 lib/file_utility.c lib/file_utility.h
                                 lib/date_utility.c lib/date_utility.h
                                 lib/green_pass_utility.c lib/green_pass_utility.h)


