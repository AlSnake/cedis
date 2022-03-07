#include "cedis/cmd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// error return 1, success ret 0
int handle_command_command(cedis_command_t *command)
{
	return 0;
}

int handle_info_command(cedis_command_t *command)
{
	puts("Cedis is an Implementation of Redis Server");
	puts("Author: AlSnake");
	puts("Project: https://github.com/AlSnake/cedis");
	return 0;
}

int handle_ping_command(cedis_command_t *command)
{
	if (!command->args[0]) {
		printf("NO ARGS\n");
	}

	printf("PING COMMAND\n");
	return 0;
}

void cedis_dump_command(cedis_command_t *command)
{
	printf("COMMAND: %s\n", command->cmd);
	for (size_t i = 0; command->args[i] != NULL; i++)
		printf("ARG: %s\n", command->args[i]);
}

int cedis_handle_command(cedis_command_t *command)
{
	size_t cmd = 0;
	const char *commands[] = { "command", "info", "ping" };

	for (size_t i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
		if (!strcmp(command->cmd, commands[i])) {
			cmd = i + 1;
			break;
		}
	}

	switch (cmd) {
	case 1:
		return handle_command_command(command);
	case 2:
		return handle_info_command(command);
	case 3:
		return handle_ping_command(command);
	default:
		return -1;
	}

	return -1;
}
