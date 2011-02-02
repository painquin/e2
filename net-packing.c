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


