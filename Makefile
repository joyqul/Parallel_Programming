CC = clang++
CFLAGS = -Wall

all: pi.cpp
	$(CC) $(CFLAGS) -o hw1_pthread pi.cpp 
