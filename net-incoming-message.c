/*
   net.c - network infrastructure for e2
*/

#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "e2.h"
#include "net.h"

// todo: pool?
void net_free_incoming_message(net_incoming_message *msg)
{
	if (msg->buffer) free(msg->buffer);
	msg->buffer = NULL;
	free(msg);
}
