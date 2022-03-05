#include "server/cedis_server.h"
#include <malloc.h>
#include <clog/clog.h>
#include <unistd.h>

cedis_server_t *cedis_server_init(void)
{
	cedis_server_t *cedis = malloc(sizeof(cedis_server_t));
	if (!cedis) {
		perror("malloc");
		return NULL;
	}

	socket_t sockconf = { .domain = AF_INET,
			      .type = SOCK_STREAM,
			      .protocol = 0,
			      .host = INADDR_ANY,
			      .port = 6379,
			      .backlog = 255 };

	cedis->tcp_server = tcp_server_init(sockconf);
	if (!cedis->tcp_server) {
		CLOG_FATAL("failed to init tcp server");
		return NULL;
	}

	return cedis;
}

void cedis_server_free(cedis_server_t *server)
{
	if (server) {
		tcp_server_free(server->tcp_server);
		free(server);
	}
}

int cedis_server_run(cedis_server_t *server)
{
	struct sockaddr *sa = (struct sockaddr *)server->tcp_server->sa;
	socklen_t addr_len = (socklen_t)sizeof(server->tcp_server->sa);

	CLOG_INFO("Cedis Running on Port: %d", server->tcp_server->sock->port);

	while (1) {
		int clifd = accept(server->tcp_server->sockfd, sa, &addr_len);
		if (clifd == -1) {
			perror("accept");
			continue;
		}

		printf("Client Connected: %d\n", clifd);
		close(clifd);
	}

	if (shutdown(server->tcp_server->sockfd, SHUT_RDWR) == -1)
		perror("shutdown");

	return 0;
}
