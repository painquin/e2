#include <stdio.h>
#include <stdlib.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "e2.h"
#include "net.h"

int shutdown_counter = 0;

void db_init();
void db_shutdown();

void net_init();
void net_shutdown();

int main(int argc, char **argv)
{

	e2log(LOG_MON, "Starting up.");

	db_init();

	net_init(52883);

	e2log(LOG_MON, "Ready.");
	while(shutdown_counter != 1)
	{


		if (shutdown_counter > 1)
		{
			--shutdown_counter;
		}
		sleep(0);
	}
	
	net_shutdown();

	db_shutdown();

	e2log(LOG_MON, "Exiting.");


	return 0;

}

void db_init()
{
	e2log(LOG_MON, "Connecting to database.");

}

void db_shutdown()
{
	e2log(LOG_MON, "Disconnecting from database.");

}


net_server *server = NULL;
void net_init(int port)
{
	if (server != NULL)
	{
		e2log(LOG_ERR, "Duplicate call to net_init.");
		exit(1);
	}

	server = net_create_server(52883, "e2");
	if (server == NULL)
	{
		e2log(LOG_ERR, "Failed to init server.");
		exit(1);
	}

	e2log(LOG_MON, "Initializing network.");

	net_start_server(server, 10);

}

void net_shutdown()
{
	e2log(LOG_MON, "Shutting down network.");
	net_stop_server(server, "Shutting down");
	net_free_server(server);
}

