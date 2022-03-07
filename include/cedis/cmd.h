#ifndef _CEDIS_CMD_H_
#define _CEDIS_CMD_H_

typedef struct cedis_command {
	char *cmd;
	char **args;
} cedis_command_t;

typedef struct cedis_command_response {
	int status;
	void *data;
} cedis_command_res_t;

void cedis_dump_command(cedis_command_t *command);
cedis_command_res_t *cedis_handle_command(cedis_command_t *command);

#endif
