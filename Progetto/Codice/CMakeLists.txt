cmake_minimum_required(VERSION 3.23)
project(Codice C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

# ==========================
# =      Eseguibili        =
# ==========================
# Aggiungiamo all'elenco dei file eseguibili singolarmente il:
# - client vaccinato
add_executable(client_vaccinato vaccinated_client.c
                                lib/sockets_utility.c lib/sockets_utility.h
                                lib/menu_utility.c lib/menu_utility.h
                                lib/data_utility.c lib/data_utility.h
                                lib/bool_utility.h)

# - client amministratore
add_executable(client_amministratore administrator_client.c)

# - client revisore
add_executable(client_revisore reviser_client.c)

# - server centro vaccinale
add_executable(server_centro_vaccinale vaccination_center_server.c
                                       lib/thread_utility.c lib/thread_utility.h
                                       lib/sockets_utility.c lib/sockets_utility.h)

# - server centrale
add_executable(server_centrale central_server.c)

# - server assistente
add_executable(server_assistente assistant_server.c)


