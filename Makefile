# Arithmetic Scheme interpreter
# Author: Ghalib Suleiman
# Time-stamp: <2008-10-14 01:05:11 ghalib>

CC = g++
CFLAGS = -ansi -pedantic -Wall -O2 -c
LFLAGS = -Wall -O2
OBJS = util.o parser.o scheme.o main.o

all: scheme

scheme: $(OBJS) 
	$(CC) $(LFLAGS) $(OBJS) -o $@

util.o: util.h util.cpp
	$(CC) $(CFLAGS) util.cpp

parser.o: util.h parser.h  parser.cpp 
	$(CC) $(CFLAGS) parser.cpp

scheme.o: util.h parser.h scheme.h scheme.cpp 
	$(CC) $(CFLAGS) scheme.cpp

main.o: util.h parser.h scheme.h main.cpp
	$(CC) $(CFLAGS) main.cpp

clean:
	\rm *.o scheme 