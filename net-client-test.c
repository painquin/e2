
#include <stdio.h>

#include "e2.h"
#include "net.h"


int main(int argc, char **argv)
{
	int running = 1;
	net_client *client = net_create_client("e2");

	net_client_connect(client, "localhost", 52883);


	while(running)
	{
		while(net_client_available(client) > 0)
		{
			net_incoming_message *imsg = net_client_get_message(client);
			switch(imsg->type)
			{
				case NetMsg_StatusChanged:
					{
						char status = net_message_read_byte(imsg);
						e2log(LOG_MON, "Status changed: %s.", net_status_names[status]);
						if (status == NetStatus_Connected)
						{
							net_outgoing_message *msg = net_client_create_message(0);
							net_msg_write_string(msg, "Hello, World!");
							net_client_send_message(client, msg);
							net_free_outgoing_message(msg);
						}
						break;
					}
				case NetMsg_Data:
					{
						char *line = net_msg_read_string(imsg);
						e2log(LOG_MON, "Received line: %s.", line);
						free(line);
						break;
					}
				default:
					e2log(LOG_ERR, "Unhandled Message Type: %s.", net_message_names[imsg->type]);
					break;
			}
			net_free_incoming_message(imsg);
		}
		sleep(100);
	}



	return 0;
}
