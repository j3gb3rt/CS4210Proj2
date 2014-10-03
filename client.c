#include <stdio.h>

#include "remote_service.h"

int main(int argc, char *argv[]) {
    int failure;
	
	failure = remote_service_client_init();
	printf("failure? %i\n", failure);
	printf("%x\n", remote_service_add(6, 9));
}