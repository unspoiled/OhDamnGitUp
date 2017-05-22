CC=gcc

# GCC flags found by running the command: 
# "gcc -mcpu=native -march=native -Q --help=target" on the raspberry pi
CFLAGS=-march=armv6zk -mcpu=arm1176jz-s -mfloat-abi=hard -mfpu=vfp

all: DisplayCommTest

DisplayCommTest: display_comm_test.c
	$(CC) -o $@ $^ $(CFLAGS)
