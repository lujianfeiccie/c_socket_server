OBJS=main.o input.o suinput.o socketserver.o othertool.o
CC=gcc
phone_server:$(OBJS)
	$(CC) $(OBJS) -o phone_server -lpthread
main.o:main.c event/input.h
	$(CC) -c main.c
input.o:event/input.c event/input.h
	$(CC) -c event/input.c
suinput.o:event/suinput.c event/suinput.h
	$(CC) -c event/suinput.c
socketserver.o:socket/socketserver.c socket/socketserver.h
	$(CC) -c socket/socketserver.c
othertool.o:util/othertool.c util/othertool.h
	$(CC) -c util/othertool.c
.PHONY:clean
clean:
	rm *.o phone_server
