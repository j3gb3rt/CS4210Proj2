#include <sys/types.h>

typedef struct __request_queue
{
	key_t shared_mem_key;
	struct __request_queue *next;
	struct __request_queue *last;
} request_queue;

typedef struct __process_queue
{
	pid_t pid;
	struct __process_queue *next;
	struct __process_queue *last;
   	request_queue *requests;
} process_queue;

void msq_watch();
void add_to_queue(key_t key, pid_t pid);
void add_to_requestq(process_queue *queue, key_t key);
