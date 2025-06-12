🛠️ Building the Chatroom – Makefile Tutorial
This project uses a Makefile to automate the compilation of the client and server programs.

📄 Makefile Contents
makefile
Copy
Edit
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

bash
Copy
Edit
make clean
📦 What Each Line Does
Line	Explanation
CC = gcc	Sets the compiler to GCC
CFLAGS = -Wall -pthread	Enables warnings and links the pthread library
all: $(TARGETS)	Default rule: builds both server and client
server: server.c	Defines how to build the server executable
client: client.c	Defines how to build the client executable
clean:	Rule to delete all compiled files
