🛠️ Building the Chatroom – Makefile Tutorial
This project uses a Makefile to automate the compilation of the client and server programs.


# Makefile for Chatroom in C

CC = gcc
CFLAGS = -Wall -pthread
TARGETS = server client

all: $(TARGETS)

server: server.c
	$(CC) $(CFLAGS) -o server server.c

client: client.c
	$(CC) $(CFLAGS) -o client client.c

clean:
	rm -f $(TARGETS)
🔧 How to Use
To compile both server and client:

bash
Copy
Edit
make
This generates two executable files:

server

client

To compile only one:

bash
Copy
Edit
make server
# or
make client
To clean/remove all built files:

