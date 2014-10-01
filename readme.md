Start by using a mutex_lock and 1 client access per shared argument stack

1. Make makefile

	`Assigned to: Jonathan`

2. Look into msg queues to make the two processes talk to each other
	
	`Assigned to:`

##Server Tasks

1. Design a struct for the process queue

	Needs:
	*	requestlist
	*	head should know 
		*	length
		*	PID
		*	next
		*	arguement queue pointer		
		*	lock pointer(or semaphone)

	`Assigned to:`
	
2. write the add method
	
	`Assigned to:`

3. initialize itself as a server
	
	`Assigned to:`

##Client Tasks

1. initialize inself as a client
	
	`Assigned to:`

2. Call add method through library
	
	`Assigned to:`

##Shared Memory Library

1. Create arguement stack struct

	Needs:
	*	lock pointer
	*	arguement pointer
	*	number of args
	*	return pointer

	`Assigned to: Jonathan`


