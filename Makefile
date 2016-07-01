CC=gcc
XNAME=nestor
MACROS=-DNESTOR_DEBUG -DNESTOR_BREAK 
MFILE=-imacros macros.h
all: main.o op.o op_modes.o nestor.o main.o graphi.o
	${CC} -o ${XNAME} main.o op.o op_modes.o nestor.o graphi.o  -lSDL2

main.o:
	${CC} ${MFILE} -g -c main.c
nestor.o:
	${CC} ${MFILE} -g -c nestor.c
op_modes.o:
	${CC} ${MFILE} -g -c op_modes.c
op.o:
	${CC} ${MFILE} -g -c op.c
op_tests.o:
	${CC} ${MFILE} -g -c op_tests.c
graphi.o:
	${CC} ${MFILE} -g -c graphi.c

val: all
    valgrind --leak-check=full ./${XNAME}


test-c: op.o op_modes.o nestor.o op_tests.o
	${CC} -o ${XNAME}_tests op.o op_modes.o nestor.o op_tests.o 

test: test-c
	./{XNAME}_tests

clean:
	rm op.o op_modes.o nestor.o op_tests.o main.o graphi.o
