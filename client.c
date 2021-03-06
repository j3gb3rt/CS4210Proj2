#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "remote_service.h"

void *remote_add(void *arguments) {
	int failure;
	int *result;
	int *args;
	
	args = (int *) arguments;
	
	failure = remote_service_client_init((unsigned int) args[0]);
	printf("failure? %i\n", failure);

	result = malloc(sizeof(int));
	*result = remote_service_add((unsigned int) args[0], args[1], args[2]);

	return result;
}

int main(int argc, char *argv[]) {
	int pthread_error;
	
	if (argc != 2) {
		printf("This program takes exactly 1 integer argument.\nThis "
			   "integer is the number of responses requested.\n");
		return -1;
	}
	
	int number_of_requests = atoi(argv[1]);
	printf("Number of requests: %i\n", number_of_requests);
	
	pthread_t threads[number_of_requests];
	int thread_args[3 * number_of_requests];
	void *results[number_of_requests];
	int i;
	
	for (i = 0; i < number_of_requests; i++) {
		thread_args[(3 * i)] = i;
		thread_args[(3 * i) + 1] = (2 * i);
		thread_args[(3 * i) + 2] = (2 * i) + 1;
		printf("Creating request thread %d with args %i & %i\n", i, thread_args[(3 * i) + 1], thread_args[(3 * i) + 2]);
		pthread_error = pthread_create(&threads[i], NULL, remote_add, &thread_args[(3 * i)]);
		if (pthread_error) {
			threads[i] = (pthread_t) 0;
			printf("Thread failed");
		}
	}
	
	for (i = 0; i < number_of_requests; i++) {
		if (threads[i] != (pthread_t) 0) {
			pthread_error = pthread_join(threads[i], &results[i]);
			if (pthread_error) {
				printf("Thread could not join");
			} else {
				printf("Request thread %d is completed with result %i\n", i, *(int *)results[i]);
				free(results[i]);
			}
		}
   }
}
