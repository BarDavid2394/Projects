CC=gcc
CLINK=$(CC)
CFLAGS=-g -Wall -std=c99
OBJS=grades.o
LIBS = -llinked-list
RM=*.o libgrades.so

libgrades.so: $(OBJS)
	$(CLINK) -shared $(OBJS) -o libgrades.so $(LIBS) -L.

grades.o : grades.c grades.h linked-list.h
	$(CC) -c -fpic $(CFLAGS) grades.c

clean:
	rm -rf $(RM) 

