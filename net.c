/*
   net.c - network infrastructure for e2
*/

#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <pthread.h>

#include "e2.h"
#include "net.h"

/*
	Note: this function is IPv4 for now
*/
net_server *net_create_server(short int port, const char *application_name)
{
	char buff[8];
	int status;
	struct addrinfo hints, *res;
	net_server *ret = malloc(sizeof(net_server));

	ret->config.port = port;
	ret->config.application_name = application_name;
	
	ret->socket = socket(AF_INET, SOCK_DGRAM, 0);

	if (ret->socket == -1)
	{
		perror("socket");
		free(ret);
		return NULL;
	}
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

	snprintf(buff, 8, "%u", port);

	status = getaddrinfo(NULL, buff, &hints, &res);
	if (status != 0)
	{
		e2log(LOG_ERR, "getaddrinfo: %s", gai_strerror(status));
		free(ret);
		return NULL;
	}

	if (bind(ret->socket, res->ai_addr, res->ai_addrlen) == -1)
	{
		perror("bind");
		free(ret);
		freeaddrinfo(res);
		return NULL;
	}

	freeaddrinfo(res);
	return ret;
}

void net_free_server(net_server *server)
{
	if (server)
	{
		free(server);
	}
}

static void * net_run_server(void *arg)
{
	net_server *server = (net_server *)arg;
	while(server->running) {


		sleep(0);
	}
	return NULL;
}

void net_start_server(net_server *server, int backlog)
{
	if (!server)
	{
		e2log(LOG_ERR, "net_start_server: null server");
		return;
	}

	server->running = 1;
	if (pthread_create(&server->net_listener, NULL, &net_run_server, server)) {
		e2log(LOG_ERR, "net_start_server: failed creating thread");
		return;
	}
}

void net_stop_server(net_server *server, const char *reason)
{
	server->running = 0;
	pthread_join(server->net_listener, NULL);
}


