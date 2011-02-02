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

