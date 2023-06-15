CXX=g++
CXXLINK=$(CXX)
CXXFLAGS=-g -Wall -std=c++11
OBJS=string.o port.o ip.o
LIBS=-L. -linput -L. -lfirewall
RM=*.o libfirewall.so firewall.exe


firewall.exe: main.o libfirewall.so libinput.so
	$(CXXLINK) -g main.o $(LIBS) -o firewall.exe

libfirewall.so: string.o port.o ip.o
	$(CXX) -shared $(OBJS) -o libfirewall.so

string.o: string.cpp string.h
	$(CXX) -c $(CXXFLAGS) -fpic string.cpp

main.o: main.cpp string.cpp ip.cpp port.cpp
	$(CXX) -c $(CXXFLAGS) -fpic main.cpp

port.o: port.cpp port.h
	$(CXX) -c $(CXXFLAGS) -fpic port.cpp

ip.o: ip.cpp ip.h
	$(CXX) -c $(CXXFLAGS) -fpic ip.cpp

clean:
	rm -rf $(RM)














