all: compile

compile: compile-sender compile-receiver

compile-sender: sender.c
	gcc sender.c -o sender

compile-receiver: receiver.c
	gcc receiver.c -o receiver
