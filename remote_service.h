#include <pthread.h>
#include <sys/types.h>

typedef struct __arg_stack
{
	int arg0;
    int arg1;
} arg_stack;



typedef struct __shared_block
{
	int locked;
    int arg0;
	int arg1;
   	int ret_val;
} shared_block;

int remote_service_server_init();
int remote_service_client_init();
int remote_service_add(int first_number, int second_number);