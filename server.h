#include <sys/types.h>

typedef struct __request_queue
{
	shared_block *shm;
	struct __request_queue *next;
	struct __request_queue *last;
} request_queue;

typedef struct __process_queue
{
	pid_t pid;
	struct __process_queue *previous;
	struct __process_queue *next;
	struct __process_queue *last;
   	request_queue *requests;
} process_queue;

void msq_watch();
void add_to_queue(shared_block *shm, pid_t pid);
void add_to_requestq(process_queue *queue, shared_block *shm);
