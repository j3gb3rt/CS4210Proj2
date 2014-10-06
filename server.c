#include "remote_service.h"
#include <pthread.h>

pthread_t msg_watcher;
process_queue queue;


void msgq_watch()
{
	key_t key;
	msg_t message;
	while(1)
	{
		msgq_rcvr(message);
		memcpy(key, message.mtext,[MSG_SIZE]); 
		add_to_queue(key, (pid_t)(message.mtype));		
	}
}

void add_to_queue(key_t key, pid_t pid)
{
	process_queue curr = queue;
	if(curr == NULL)
	{
		//if process queue is NULL, create first node
		queue = (process_queue *)malloc(sizeof(process_queue));
		queue.pid = pid;
		queue->last = queue;
		curr = queue;
	}	

	//iterate through the process queue, until pid is a match
	while(curr->next != NULL)
	{
		if(curr.pid != pid)
		{
			curr = curr->next;
		}else
		{
			break;
		}
	}

	//process does not exist in queue, so add it
	if(curr.pid != pid)
	{
		queue->last->next = (process_queue *)malloc(sizeof(process_queue));
		queue->last = queue->last->next;
		queue->last.pid = pid;
		curr = queue->last;
	}

	add_to_requestq(curr, key)
}

void add_to_requestq(process_queue queue, key_t key)
{
	request_queue curr = queue->requests;
	//request queue empty, add to front
	if(curr == NULL)
	{
		queue->requests = (request_queue *)malloc(sizeof(request_queue));
		queue->requests->last = queue->requests;
		queue->requests.shared_mem_key = key;	
	}else
	{
	//create and add to end of request queue
		curr = (request_queue *)malloc(sizeof(request_queue));
		curr.shared_mem_key = key;
		queue->requests->last->next = curr;
		queue->request->last = curr;
	}
}


main(1)
{
	remote_service_server_init();
	pthread_create(&msg_watcher, NULL, &msg_watch, NULL);
        fprint("message queue watching thread created\n"); 
	/*TODO service requests*/
}
