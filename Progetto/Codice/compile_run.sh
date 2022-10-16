#! /bin/sh

make build
x-terminal-emulator make run_central_server
x-terminal-emulator make run_assistant_server
x-terminal-emulator make run_vaccinated_server
x-terminal-emulator make run_vaccinated_client
x-terminal-emulator make run_reviser_client
x-terminal-emulator make run_administrator_client
