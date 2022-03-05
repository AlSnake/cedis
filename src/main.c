#include "server/cedis_server.h"
#include <clog/clog.h>

int main(void)
{
	cedis_server_t *cedis_server = cedis_server_init();
	if (!cedis_server) {
		CLOG_FATAL("failed to init cedis server");
		return 1;
	}

	if (cedis_server_run(cedis_server) == -1) {
		CLOG_FATAL("failed to run cedis server");
		return 1;
	}

	return 0;
}
