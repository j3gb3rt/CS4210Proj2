#include "remote_service.h"
#include "server.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

pthread_t msg_watcher;
process_queue *queue;

void msgq_watch()
{
	key_t key;
	msg_t message;
	while(1)
	{
		msgq_rcvr(message);
		memcpy(&key, message.mtext,MSG_SIZE); 
		add_to_queue(key, (pid_t)(message.mtype));		
	}
}

void add_to_queue(key_t key, pid_t pid)
{
	process_queue *curr = queue;
	if(curr == NULL)
	{
		//if process queue is NULL, create first node
		queue = (process_queue *)malloc(sizeof(process_queue));
		queue->pid = pid;
		queue->last = queue;
		curr = queue;
	}	

	//iterate through the process queue, until pid is a match
	while(curr->next != NULL)
	{
		if(curr->pid != pid)
		{
			curr = curr->next;
		}else
		{
			break;
		}
	}

	//process does not exist in queue, so add it
	if(curr->pid != pid)
	{
		queue->last->next = (process_queue *)malloc(sizeof(process_queue));
		queue->last = queue->last->next;
		queue->last->pid = pid;
		curr = queue->last;
	}

	add_to_requestq(curr, key);

}

void add_to_requestq(process_queue *queue, key_t key)
{
	request_queue *curr = queue->requests;
	//request queue empty, add to front
	if(curr == NULL)
	{
		queue->requests = (request_queue *)malloc(sizeof(request_queue));
		queue->requests->last = queue->requests;
		queue->requests->shared_mem_key = key;	
	}else
	{
	//create and add to end of request queue
		curr = (request_queue *)malloc(sizeof(request_queue));
		curr->shared_mem_key = key;
		queue->requests->last->next = curr;
		queue->requests->last = curr;
	}
}


int main(int argc, char *argv[])
{
	process_queue *curr_process;

	remote_service_server_init();
	pthread_create(&msg_watcher, NULL, (void *)&msgq_watch, NULL);
    printf("message queue watching thread created\n");
	
	while(1) {
		/*TODO iterate through the process queue
			   and add and store result unlock and
			   detach */
		curr_process = queue;
		if (curr_process != NULL) {
			do {
				int shared_mem_identifier;
				shared_block *shared_mem;
				request_queue *request;
				
				request = curr_process->requests;
					
				shared_mem_identifier = shmget(request->shared_mem_key, sizeof(shared_block), 
						   					   S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP );
				shared_mem = (shared_block *) shmat(shared_mem_identifier, NULL, 0);
				shared_mem->ret_val = shared_mem->arg0 + shared_mem->arg1;
				shared_mem->locked = 0;
				shmdt(shared_mem);
				
				if (request->next != NULL) {
					curr_process->requests = request->next;
				} else  {
				
				}
				
				curr_process = curr_process->next;
			} while (curr_process != NULL);
		}
	}
}
