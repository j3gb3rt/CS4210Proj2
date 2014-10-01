#include <pthread.h>

typedef struct __arg_stack
{
	int arg0;
    int arg1;
} arg_stack;



typedef struct __shared_block
{
	mutex_thread_t *lock;
    arg_stack *args;
    int arg_count;
    int *ret_location;
} shared_block;