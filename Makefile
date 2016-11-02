CC=gcc
XNAME=nestor
MACROS=-DNESTOR_DEBUG -DNESTOR_BREAK 
MFILE=-imacros macros.h
CFLAGS=-g
all: main.o op.o op_modes.o nestor.o main.o graphi.o ppu_registers.o
	${CC} ${CFLAGS} -o ${XNAME} main.o op.o op_modes.o nestor.o graphi.o ppu_registers.o -lSDL2 -pthread

ppu_registers.o:
	${CC} ${CFLAGS} -c ppu_registers.c
main.o:
	${CC} ${MFILE} ${CFLAGS} -c main.c -pthread
nestor.o:
	${CC} ${MFILE} ${CFLAGS} -c nestor.c
op_modes.o:
	${CC} ${MFILE} ${CFLAGS} -c op_modes.c
op.o:
	${CC} ${MFILE} ${CFLAGS} -c op.c
op_tests.o:
	${CC} ${MFILE} ${CFLAGS} -c op_tests.c
graphi.o:
	${CC} ${MFILE} ${CFLAGS} -c graphi.c

val: all
	valgrind --leak-check=full ./${XNAME}

test-c: op.o op_modes.o nestor.o op_tests.o
	${CC} -o ${XNAME}_tests op.o op_modes.o nestor.o op_tests.o 

test: test-c
	./{XNAME}_tests

clean:
	rm op.o op_modes.o nestor.o main.o graphi.o ppu_registers.o
