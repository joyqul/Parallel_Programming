CC = clang++
CFLAGS = -Wall

all: hw01PI.cpp
	$(CC) $(CFLAGS) -o hw1_pthread hw01PI.cpp 
