#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "remote_service.h"



int remote_service_init() {
    FILE *file;
	key_t shared_mem_key;
    int shared_mem_identifier;
	shared_block *shared_mem;
    
	
	file = fopen("/dev/shm/remote_service", "w+");
	if (file != NULL) {
		printf("%p\n", file);
		fclose(file);
	} else {
		printf("%s\n", strerror(errno));
		return -1;
	}
    shared_mem_key = ftok("/dev/shm/remote_service", 14);
	if (shared_mem_key != -1) {
		printf("%i\n", shared_mem_key);
	} else {
		printf("%s\n", strerror(errno));
		return -2;
	}
    shared_mem_identifier = shmget(shared_mem_key, sizeof(shared_block), 
								   IPC_CREAT | S_IWUSR | S_IRUSR | 
								   S_IWGRP | S_IRGRP );
    if (shared_mem_identifier != -1) {
		printf("%i\n", shared_mem_identifier);
	} else {
		printf("%s\n", strerror(errno));
		return -3;
	}
	shared_mem = (shared_block *) shmat(shared_mem_identifier, NULL, 0);
	if (shared_mem != (void *) -1) {
		printf("%p\n", shared_mem);
	} else {
		printf("%s\n", strerror(errno));
		return -4;
	}
	//created shared memory block
    //enable connection through message queue
    
    

	return 0;
}