#include <sys/types.h>
#include <sys/socket.h>

#include "fixed.h"

#define MSG_RELIABLE 0x1
#define MSG_ORDERED 0x2

typedef struct net_cfg_t net_configuration;
typedef struct net_con_t net_connection;
typedef struct net_srv_t net_server;
typedef struct net_cln_t net_client;

typedef struct net_msgo_t net_outgoing_message;
typedef struct net_msgi_t net_incoming_message;

enum message_type
{
	NetMsg_Error = 0,
	
	NetMsg_Connect,
	NetMsg_ConnectResponse,
	NetMsg_StatusChanged,

	NetMsg_Data
};


typedef char net_msg_flags;

struct net_msgo_t
{
	net_msg_flags flags;

	int allocated_length;
	int pos;

	char *buffer;
};

struct net_msgi_t
{
	enum message_type type;
	net_msg_flags flags;

	int len;
	int pos;

	char *buffer;
};

struct net_cfg_t
{
	short int port;
	const char *application_name;
};

struct net_con_t
{
	struct sockaddr remote;

};

struct net_srv_t
{
	net_configuration config;
	int socket;
	pthread_t net_listener;
	char running;


};


extern net_server *net_create_server(int port, const char *application_name);
extern void net_free_server(net_server *server);
extern void net_start_server(net_server *server, int backlog);

extern net_outgoing_message *net_create_message(net_msg_flags flags);
extern void net_free_incoming_message(net_incoming_message *msg);
extern void net_free_outgoing_message(net_outgoing_message *msg);

// Integer data types
extern void net_msg_write_byte(net_outgoing_message *msg, char data);
extern void net_msg_write_int16(net_outgoing_message *msg, int16_t data);
extern void net_msg_write_int32(net_outgoing_message *msg, int32_t data);

extern void net_msg_write_string(net_outgoing_message *msg, const char *data);

extern void net_msg_write_fix(net_outgoing_message *msg, fix data);


extern char net_msg_read_byte(net_incoming_message *msg);
extern int16_t net_msg_read_int16(net_incoming_message *msg);
extern int32_t net_msg_read_int32(net_incoming_message *msg);
extern char *net_msg_read_string(net_incoming_message *msg);
extern fix net_msg_read_fix(net_incoming_message *msg);
