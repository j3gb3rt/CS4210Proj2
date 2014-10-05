#include "remote_service.h"
#include <pthread.h>

pthread_t msg_watcher;



void msgq_watch()
{
	key_t key;
	msg_t message;
	while(1)
	{
		msgq_rcvr(message);
		memcpy(key, message.mtext,[MSG_SIZE]); 
		
		/*TODO add request to request queue*/
	}
}



int main(int argc, char *argv[])
{
	remote_service_server_init();
	pthread_create(&msg_watcher, NULL, &msg_watch, NULL);
    fprint("message queue watching thread created\n");
	while(1) {
		/*TODO interate through the process queue
			   and add and store result unlock and
			   detach */
	}
}
