#ifndef _CEDIS_PARSER_H_
#define _CEDIS_PARSER_H_

#include "cmd.h"

typedef enum CEDIS_TYPE {
	SIMPLE_STRING,
	ERROR,
	INTEGER,
	BULK_STRING,
	ARRAY,
	NIL
} CEDIS_TYPE_T;

typedef struct cedis_request {
	cedis_command_t *command;
} cedis_request_t;

cedis_request_t *cedis_parse_request(const char *data);

#endif
