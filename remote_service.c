#include <sys/ipc.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/msg.h>

#include "remote_service.h"

#define MSG_Q_KEY 1111

int msg_q_id;
key_mem_pair pairs[MAX_THREADS];
key_t msg_q_key;


int remote_service_server_init() {
	FILE *file;
	pthread_t msg_watcher;
	int flag = IPC_CREAT | 0666;

	file = fopen("remote_service", "W+");
	if(file != NULL){
		fclose(file);
	}

	msg_q_key = ftok("remote_service", 'a');

	if((msg_q_id = msgget(msg_q_key, flag)) < 0)
	{
		perror("msgget");
		return -1;
	}else
	{
		printf("msgget: msgget succeeded: msg_q_id = %d\n", msg_q_id);
	}
	return 0;

}

int remote_service_client_init(unsigned int id) {
	FILE *file;
    int shared_mem_identifier;
	key_t msg_q_key;
	
	if (id > MAX_THREADS) {
		return -1;
	}
		
	file = fopen("/dev/shm/remote_service", "w+");
	if (file != NULL) {
//		printf("%p\n", file);14
		fclose(file);
	} else {
//		printf("%s\n", strerror(errno));
		return -2;
	}
    pairs[id].shared_mem_key = ftok("/dev/shm/remote_service", id);
	if (pairs[id].shared_mem_key != -1) {
//		printf("%i\n", shared_mem_key);
	} else {
//		printf("%s\n", strerror(errno));
		return -3;
	}
    shared_mem_identifier = shmget(pairs[id].shared_mem_key, sizeof(shared_block), 
								   IPC_CREAT | S_IWUSR | S_IRUSR | 
								   S_IWGRP | S_IRGRP );
    if (shared_mem_identifier != -1) {
//		printf("%i\n", shared_mem_identifier);
	} else {
//		printf("%s\n", strerror(errno));
		return -4;
	}
	pairs[id].shared_mem = (shared_block *) shmat(shared_mem_identifier, NULL, 0);
	if (pairs[id].shared_mem != (void *) -1) {
//		printf("%p\n", shared_mem);
	} else {
//		printf("%s\n", strerror(errno));
		return -5;
	}
	
	msg_q_key = ftok("remote_service", 'a');
	msg_q_id = msgget(msg_q_key, 0666);
	if (msg_q_id != -1) {
		printf("%i\n", msg_q_id);
	} else {
		printf("%s\n", strerror(errno));
		return -5;
	}
	return 0;
	
}

int remote_service_add(unsigned int id, int first_number, int second_number) {
	pairs[id].shared_mem->locked = 1;
	pairs[id].shared_mem->arg0 = first_number;
	pairs[id].shared_mem->arg1 = second_number;
	pairs[id].shared_mem->ret_val = first_number + second_number;
	pairs[id].shared_mem->pid = getpid();
	

	//mgsq call
	//make a message
//	printf("%i\n", pairs[id].shared_mem_key);
	msg_t message;
	message.mtype = 1;
	memcpy(message.mtext, &pairs[id].shared_mem_key, MSG_SIZE);
	//sprintf(message.mtext, "%i", pairs[id].shared_mem_key);
	msgsnd(msg_q_id, &message, MSG_SIZE, 0);
	
	while (pairs[id].shared_mem->locked) {
		//spin
	}

	return pairs[id].shared_mem->ret_val;
}

void msgq_rcvr(msg_t *message)
{	
	msgrcv(msg_q_id, message, MSG_SIZE, 1, 0);
}






