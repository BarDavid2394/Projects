

CC = g++
CFLAGS = -std=c++11 -Wall  -g
CCLINK = $(CC)
OBJS = main.o Bank.o  ReadWrite.o
RM = rm -f
TARGET = Bank

# Creating the executable
$(TARGET): $(OBJS)
	g++ -std=c++11 -Wall -Werror -pedantic-errors -DNDEBUG -pthread *.cpp -o Bank

# Creating the object files

ReadWrite.o: ReadWrite.cpp ReadWrite.h
	$(CC) $(CFLAGS) -c ReadWrite.cpp

Bank.o: Bank.cpp Bank.h
	$(CC) $(CFLAGS) -c Bank.cpp

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

# Cleaning old files before new make
clean:
	$(RM) $(TARGET) $(OBJS) *~ "#" core.*
