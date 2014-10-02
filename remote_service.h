#include <pthread.h>

typedef struct __arg_stack
{
	int arg0;
    int arg1;
} arg_stack;



typedef struct __shared_block
{
	pthread_mutex_t *lock;
    arg_stack *args;
    int arg_count;
    int *ret_location;
} shared_block;

int remote_service_init();