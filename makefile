# PROGRAM:    Assign 7 
# PROGRAMMER: Samuel Piecz
# LOGON ID:   Z1732715
# DATE DUE:   12/02/19
#

CCFLAGS = -ggdb -Wall -std=c++11 -m64

Assign7: Assign7.o Entry.o
	g++ $(CCFLAGS) -o Assign7 Assign7.o Entry.o

Assign7.o: Assign7.cxx  Entry.h 
	g++ $(CCFLAGS) -c Assign7.cxx

Entry.o: Entry.cxx Entry.h
	g++ $(CCFLAGS) -c Entry.cxx

clean:
	-rm *.o Assign7
