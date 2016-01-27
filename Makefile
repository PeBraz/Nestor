CC=gcc
XNAME=nestor

all: op.o op_modes.o nestor.o main.o
	${CC} -o ${XNAME} op.o op_modes.o nestor.o main.o

main.o:
	${CC} -g -c main.c
nestor.o:
	${CC} -g -c nestor.c
op_modes.o:
	${CC} -g -c op_modes.c
op.o:
	${CC} -g -c op.c
op_tests.o:
	${CC} -g -c op_tests.c

val: all
    valgrind --leak-check=full ./${XNAME}


test-c: op.o op_modes.o nestor.o op_tests.o
	${CC} -o ${XNAME}_tests op.o op_modes.o nestor.o op_tests.o 

test: test-c
	./{XNAME}_tests

clean:
	rm op.o op_modes.o nestor.o op_tests.o main.o
