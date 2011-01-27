#include <sys/socket.h>
typedef struct net_cfg_t net_configuration;
typedef struct net_con_t net_connection;
typedef struct net_msg_t net_message;
typedef struct net_srv_t net_server;
typedef struct net_cln_t net_client;

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


extern net_server *net_create_server(short int port, const char *application_name);
extern void net_free_server(net_server *server);
extern void net_start_server(net_server *server, int backlog);

