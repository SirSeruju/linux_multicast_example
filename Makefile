all: compile

compile: compile-server compile-client

compile-server: server.c
	gcc server.c -o server

compile-client: client.c
	gcc client.c -o client

clean:
	rm -f server client
