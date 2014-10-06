CC = gcc
CFLAGS = -g
LDFLAGS = 
LIBS = .
SRC = remote_service.c
OBJ = $(SRC:.c=.o)

OUT = bin/libipc.a

.PHONY: all
.DEFAULT: all

all : $(OUT) client server

.c.o: 
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	@mkdir -p bin
	ar rcs $(OUT) $(OBJ)

client :
	$(CC) $(CFLAGS) client.c $(OUT) -pthread -o client

server :
	$(CC) $(CFLAGS) server.c $(OUT) -pthread -o server

#matrix:
#	$(CC) $(CFLAGS) src/gt_matrix.c $(OUT) -o bin/matrix

#all : gt_include.h gt_kthread.c gt_kthread.h gt_uthread.c gt_uthread.h gt_pq.c gt_pq.h gt_signal.h gt_signal.c gt_spinlock.h gt_spinlock.c gt_matrix.c
#	@echo Building...
#	@gcc -o matrix gt_matrix.c gt_kthread.c gt_pq.c gt_signal.c gt_spinlock.c gt_uthread.c
#	@echo Done!
#	@echo Now run './matrix'


clean :
	@rm *.o bin/*.a client server
	@echo Cleaned!
