#include<stdio.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "e2.h"

int shutdown = 0;

void db_init();
void db_shutdown();

void net_init();
void net_shutdown();

int main(int argc, char **argv)
{

	e2log(LOG_MONITOR, "Starting up.");

	db_init();

	net_init(52883);

	e2log(LOG_MONITOR, "Ready.");
	while(shutdown != 1)
	{


		if (shutdown > 1)
		{
			--shutdown;
		}
	}
	
	net_shutdown();

	db_shutdown();

	e2log(LOG_MONITOR, "Exiting.");


	return 0;

}

void db_init()
{
	e2log(LOG_MONITOR, "Connecting to database.");

}

void db_shutdown()
{
	e2log(LOG_MONITOR, "Disconnecting from database.");

}

void net_init(int port)
{
	e2log(LOG_MONITOR, "Initializing network.");

}

void net_shutdown()
{
	e2log(LOG_MONITOR, "Shutting down network.");

}
