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
#include <errno.h>

#include <pthread.h>

#include "e2.h"
#include "net.h"

/*
	Note: this function is IPv4 for now
*/
net_server *net_create_server(int port, const char *application_name)
{
	char buff[8];
	int status;
	struct addrinfo hints, *res;
	net_server *ret = malloc(sizeof(net_server));

	e2log(LOG_MON, "Creating server on port %u", (int)port);

	ret->config.port = htons(port);
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
	char buffer[1024];
	char addrbuff[INET_ADDRSTRLEN];

	net_server *server = (net_server *)arg;
	struct sockaddr from;
	int alen, rlen;
	while(server->running) {
		alen = sizeof(from);
		rlen = recvfrom(server->socket, buffer, 1024, 0, &from, &alen);
		if (rlen == -1)
		{
			e2log(LOG_ERR, "recvfrom: %s", strerror(errno));
		}
		else
		{
			buffer[rlen] = 0;
			inet_ntop(AF_INET,
					&((struct sockaddr_in *)&from)->sin_addr,
					addrbuff,
					INET_ADDRSTRLEN);

			e2log(LOG_MON, "Message from %s: %s", addrbuff, buffer);
		}
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

#define INITIAL_MSG_ALLOC 256

net_outgoing_message *net_create_message(net_msg_flags flags)
{
	net_outgoing_message *ret = malloc(sizeof(net_outgoing_message));
	ret->flags = flags;
	ret->allocated_length = INITIAL_MSG_ALLOC;
	ret->pos = 0;
	ret->buffer = malloc(INITIAL_MSG_ALLOC);

	return ret;
}

int resize_message(net_outgoing_message *msg)
{
	char *b;
	int new_size = msg->allocated_length << 1;
	// TODO check for overflow
	b = realloc(msg->buffer, new_size);
	
	if (b == NULL)
		return 1;

	msg->allocated_length = new_size;

	if (b == msg->buffer) return 0;
	msg->buffer = b;
	return 0;
}

// todo: pool?
void net_free_incoming_message(net_incoming_message *msg)
{
	if (msg->buffer) free(msg->buffer);
	msg->buffer = NULL;
	free(msg);
}

void net_free_outgoing_message(net_outgoing_message *msg)
{
	if (msg->buffer) free(msg->buffer);
	msg->buffer = NULL;
	free(msg);
}

static void require_len(net_outgoing_message *msg, int len)
{
	if (msg->pos + len <= msg->allocated_length) return;
	resize_message(msg);
}

// Integer data types
void net_msg_write_byte(net_outgoing_message *msg, char data)
{
	require_len(msg, sizeof(char));
	msg->buffer[msg->pos] = data;
	msg->pos += sizeof(char);
}

void net_msg_write_int16(net_outgoing_message *msg, int16_t data)
{
	require_len(msg, sizeof(int16_t));
	*((int16_t *)(msg->buffer + msg->pos)) = htons(data);
	msg->pos += sizeof(int16_t);
}

void net_msg_write_int32(net_outgoing_message *msg, int32_t data)
{
	require_len(msg, sizeof(int32_t));
	*((int32_t *)(msg->buffer + msg->pos)) = htonl(data);
	msg->pos += sizeof(int32_t);
}

void net_msg_write_string(net_outgoing_message *msg, const char *data)
{
	short slen = strlen(data);

	require_len(msg, slen + sizeof(int16_t));
	net_msg_write_int16(msg, slen);
	strncpy(msg->buffer + msg->pos, data, (size_t)slen);
	msg->pos += slen;
}

void net_msg_write_fix(net_outgoing_message *msg, fix data)
{
	net_msg_write_int32(msg, (int32_t)data);
}

char net_msg_read_byte(net_incoming_message *msg)
{
	return msg->buffer[msg->pos++];
}

int16_t net_msg_read_int16(net_incoming_message *msg)
{
	int16_t ret = ntohs(*((int16_t *)(msg->buffer+msg->pos)));
	msg->pos += sizeof(int16_t);
	return ret;
}

int32_t net_msg_read_int32(net_incoming_message *msg)
{
	int32_t ret = ntohl(*((int32_t *)(msg->buffer+msg->pos)));
	msg->pos += sizeof(int32_t);
	return ret;
}


char *net_msg_read_string(net_incoming_message *msg)
{
	int slen;
	char *buff;
	slen = net_msg_read_int16(msg);
	buff = malloc(slen+1);
	strncpy(buff, msg->buffer+msg->pos, slen);
	buff[slen] = 0;
	msg->pos += slen;
	return buff;
}

fix net_msg_read_fix(net_incoming_message *msg)
{
	return (fix)(net_msg_read_int32(msg));
}


