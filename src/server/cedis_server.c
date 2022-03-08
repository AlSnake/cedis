#include "server/cedis_server.h"
#include <malloc.h>
#include <clog/clog.h>
#include <unistd.h>
#include "cedis/request.h"
#include "cedis/cmd.h"
#include <string.h>
#include "cedis/encoder.h"
#include "cedis/reply.h"

// int handle_cedis(char *data)
// {
// 	return 1;
// }

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

		char buf[1024] = { 0 };
		ssize_t r = read(clifd, buf, sizeof(buf));
		if (r == -1) {
			perror("read");
			close(clifd);
			continue;
		}

		cedis_request_t *request = cedis_request_parse(buf);
		if (!request) {
			CLOG_ERROR("failed to parse cedis request");
			close(clifd);
			continue;
		}

		// cedis_command_dump(request->command);

		cedis_command_res_t *res =
			cedis_command_handle(request->command);
		if (!res) {
			char *msg =
				reply_unknown_command(request->command->cmd);
			char *encoded = resp_error_encode(msg);

			if (write(clifd, encoded, strlen(encoded)) == -1)
				perror("write");

			free(msg);
			free(encoded);
		} else if (res->status != 0) {
			char *msg = reply_custom_message(request->command->cmd,
							 "execution failed");
			char *encoded = resp_error_encode(msg);

			if (write(clifd, encoded, strlen(encoded)) == -1)
				perror("write");

			free(msg);
			free(encoded);
		}

		if (res && res->status == 0 && res->data) {
			if (write(clifd, res->data, strlen(res->data)) == -1)
				perror("write");
		}

		close(clifd);
		cedis_request_free(request);
		cedis_command_res_free(res);
	}

	if (shutdown(server->tcp_server->sockfd, SHUT_RDWR) == -1)
		perror("shutdown");

	return 0;
}
