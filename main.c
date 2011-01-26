#include<stdio.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "e2.h"

int main(int argc, char **argv)
{

	e2log(LOG_MONITOR, "Starting up.");

	// todo: Connect to database


	// todo: Set up networking

	// todo: Enter network loop while (running)
	

	// todo: Cleanup networking

	// todo: Disconnect from database

	e2log(LOG_MONITOR, "Exiting.");


	return 0;

}
