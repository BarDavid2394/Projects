CC=gcc
CLINK=$(CC)
CFLAGS=-g -Wall -std=c99
OBJS=main.o stack.o student.o
EXEC=prog.exe
RM=rm -rf

$(EXEC): $(OBJS)
	$(CLINK) $(OBJS) -o $(EXEC)

main.o : main.c stack.h student.h
	$(CC) $(CFLAGS) -c main.c

stack.o: stack.c stack.h
	$(CC) $(CFLAGS) -c stack.c

student.o: student.c student.h
	$(CC) $(CFLAGS) -c student.c

clean:
	$(RM) *.o *.exe