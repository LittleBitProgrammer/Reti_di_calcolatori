# Makefile Green Pass

# Build di tutti i client e server
build: build_central_server \
		build_assistant_server \
		build_vaccinated_server \
		build_vaccinated_client \
		build_reviser_client \
		build_administrator_client

# Build server Centrale
build_central_server:
	gcc central_server.c \
		lib/sockets_utility.c \
		lib/thread_utility.c \
		lib/bool_utility.c \
		lib/file_utility.c \
		lib/date_utility.c \
		lib/green_pass_utility.c \
		-o output/server_centrale

# Build server assistente
build_assistant_server:
	gcc assistant_server.c \
		lib/sockets_utility.c \
		lib/thread_utility.c \
		lib/bool_utility.c \
		lib/file_utility.c \
		lib/date_utility.c \
		lib/green_pass_utility.c \
		-o output/server_assistente

# Build server centro vaccinale
build_vaccinated_server:
	gcc vaccination_center_server.c \
		lib/thread_utility.c \
		lib/sockets_utility.c \
		lib/bool_utility.c \
		lib/file_utility.c \
		lib/date_utility.c \
		lib/green_pass_utility.c \
		-o output/centro_vaccinale

# Build client vaccinale
build_vaccinated_client:
	gcc vaccinated_client.c \
		lib/sockets_utility.c \
		lib/menu_utility.c \
		lib/date_utility.c \
		lib/bool_utility.c \
		lib/code_verification.c \
		lib/digits_utility.c \
		lib/file_utility.c \
		lib/buffer_utility.c \
		lib/package_utility.c \
		-o output/client_vaccinale
		

# Build client revisore
build_reviser_client:
	gcc reviser_client.c \
		lib/sockets_utility.c \
		lib/menu_utility.c \
		lib/date_utility.c \
		lib/bool_utility.c \
		lib/code_verification.c \
		lib/digits_utility.c \
		lib/file_utility.c \
		lib/buffer_utility.c \
		lib/package_utility.c \
		-o output/client_reviser

# Build client amministratore
build_administrator_client:
	gcc administrator_client.c \
		lib/sockets_utility.c \
		-o output/client_administrator

# Run del server centrale
run_central_server:
	sudo ./output/server_centrale

# Run del server assisgente
run_assistant_server:
	sudo ./output/server_assistente

# Run del server vaccinale
run_vaccinated_server:
	sudo ./output/centro_vaccinale

# Run del client vaccinale
run_vaccinated_client:
	./output/client_vaccinale localhost

# Run del client revisore
run_reviser_client:
	./output/client_reviser localhost

# Run del client amministratore
run_administrator_client:
	./output/client_administrator localhost

clean:
	rm output/*
	