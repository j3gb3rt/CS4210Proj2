#include <stdio.h>

#include "remote_service.h"

int main(int argc, char *argv[]) {
    int failure;
	
	failure = remote_service_init();
	printf("failure? %i\n", failure);
}