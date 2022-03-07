#include "cedis/cmd.h"
#include <stdio.h>
#include <stdlib.h>

void cedis_dump_command(cedis_command_t *command)
{
	printf("COMMAND: %s\n", command->cmd);
	for (size_t i = 0; command->args[i] != NULL; i++)
		printf("ARG: %s\n", command->args[i]);
}
