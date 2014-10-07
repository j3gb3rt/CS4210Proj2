#include "remote_service.h"
#include "server.h"
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>

pthread_t msg_watcher;
process_queue *queue;

void msgq_watch()
{
	key_t key;
	msg_t message;
	int shmid;
	shared_block *shm;

	while(1)
	{
		msgq_rcvr(message);
		memcpy(&key, message.mtext,MSG_SIZE); 
	
		shmid = shmget(key, sizeof(shared_block), IPC_CREAT | S_IWUSR | S_IRUSR | 
                                                              S_IWGRP | S_IRGRP);
		if(shmid == -1)
		{
			printf("%s\n", strerror(errno));
		}

		shm = (shared_block *) shmat(shmid, NULL, 0);
		if(shm == (void *) -1)
		{
			printf("%s\n", strerror(errno));
		}

		add_to_queue(shm, shm->pid);		
	}
}

void add_to_queue(shared_block *shm, pid_t pid)
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

	add_to_requestq(curr, shm);

}

void add_to_requestq(process_queue *queue, shared_block *shm)
{
	request_queue *curr = queue->requests;
	//request queue empty, add to front
	if(curr == NULL)
	{
		queue->requests = (request_queue *)malloc(sizeof(request_queue));
		queue->requests->last = queue->requests;
		queue->requests->shm = shm;	
	}else
	{
	//create and add to end of request queue
		curr = (request_queue *)malloc(sizeof(request_queue));
		curr->shm = shm;
		queue->requests->last->next = curr;
		queue->requests->last = curr;
	}
	printf("added request to queue\n");
}

void update_last(process_queue *queue)
{
	process_queue *curr_process = queue;
	while(curr_process->next != queue->last)
	{
		curr_process = curr_process->next;
	}
	queue->last = curr_process;
}


int main(int argc, char *argv[])
{
	process_queue *curr_process;
	process_queue *next_process;

	remote_service_server_init();
	pthread_create(&msg_watcher, NULL, (void *)&msgq_watch, NULL);
	printf("message queue watching thread created\n");
	
	while(1) {
		/*iterate through the process queue
			   and add and store result unlock and
			   detach */
		curr_process = queue;
		if (curr_process != NULL) {
			do {
//				int shared_mem_identifier;
				shared_block *shared_mem;
				request_queue *request;
				
				request = curr_process->requests;
					
				if(request == NULL)
				{
					printf("about to access null request\n");
				}
//				shared_mem_identifier = shmget(request->shared_mem_key, sizeof(shared_block), 
//						   					   S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP );
//				shared_mem = (shared_block *) shmat(shared_mem_identifier, NULL, 0);
				shared_mem = request->shm;
				shared_mem->ret_val = shared_mem->arg0 + shared_mem->arg1;
				shared_mem->locked = 0;
				shmdt(shared_mem);
				printf("request completed\n");
				
				if (request->next != NULL) {
					curr_process->requests->next->last = request->last;
					curr_process->requests = request->next;
					free(request);
				} else  {
//					queue empty
					if(curr_process == queue && curr_process == queue->last)
					{
						queue = NULL;
					}else if(curr_process == queue)
//					assign new node as queue head
					{
						queue = curr_process->next;
						queue->last = curr_process->last;
					}else if(curr_process == queue->last)
					{
						update_last(queue);
					}
					next_process = curr_process->next;
					free(curr_process);
					curr_process = next_process;
				}
				
			} while (curr_process != NULL);
		}
	}
}
