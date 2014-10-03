#include <sys/types.h>

typedef struct __request_queue
{
	key_t shared_mem_key;
	__request_queue *next;
} request_queue;

typedef struct __process_queue
{
	pid_t pid;
	__process_queue *next;
   	request_queue requests;
} process_queue;

