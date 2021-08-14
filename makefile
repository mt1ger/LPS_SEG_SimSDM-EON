CC=g++-11
CPPSTD=-std=c++20
LDFLAGS=#-labt
CFLAGS=-c -Wall
DEBUGFLAGS= -gdwarf-3
# EXEC=Sim_Firstfit
EXEC=test
INCLUDES=#-I /Users/mt1ger/argobots/usr/include\
				 -I /Users/mt1ger/argobots-install/include
LIBS=#-L /Users/mt1ger/argobots-install/lib


SRCS=Network.cpp\
	 ModulationFormats.cpp\
	 Dijkstra.cpp\
	 RoutingTable.cpp\
	 TrafficGenerator.cpp\
	 Event.cpp\
	 EventQueue.cpp\
	 ResourceAssignment_Firstfit.cpp\
	 RandomVariable.cpp\
	 Topology.cpp\
	 main.cpp

OBJS=$(SRCS:.cpp=.o)
CLEANFILES=*.o $(EXEC)

all: $(OBJS) $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CPPSTD) $(DEBUGFLAGS) $(INCLUDES) $(LDFLAGS) $(LIBS) $(OBJS) -o $@

Network.o: Network.cpp Network.h EventQueue.h RoutingTable.h TrafficGenerator.h Event.h ResourceAssignment_Firstfit.h
	$(CC) $(CPPSTD) $(DEBUGFLAGS) $(INCLUDES) $(LDFLAGS) $(LIBS) $(CFLAGS) Network.cpp

Dijkstra.o: Dijkstra.cpp Dijkstra.h Network.h Topology.h
	$(CC) $(CPPSTD) $(DEBUGFLAGS) $(INCLUDES) $(LDFLAGS) $(LIBS) $(CFLAGS) Dijkstra.cpp

RoutingTable.o: RoutingTable.cpp RoutingTable.h Network.h Dijkstra.h
	$(CC) $(CPPSTD) $(DEBUGFLAGS) $(INCLUDES) $(LDFLAGS) $(LIBS) $(CFLAGS) RoutingTable.cpp

TrafficGenerator.o: TrafficGenerator.cpp TrafficGenerator.h Network.h EventQueue.h RandomVariable.h
	$(CC) $(CPPSTD) $(DEBUGFLAGS) $(INCLUDES) $(LDFLAGS) $(LIBS) $(CFLAGS) TrafficGenerator.cpp

Event.o: Event.cpp Event.h
	$(CC) $(CPPSTD) $(DEBUGFLAGS) $(INCLUDES) $(LDFLAGS) $(LIBS) $(CFLAGS) Event.cpp

EventQueue.o: EventQueue.cpp EventQueue.h Event.h
	$(CC) $(CPPSTD) $(DEBUGFLAGS) $(INCLUDES) $(LDFLAGS) $(LIBS) $(CFLAGS) EventQueue.cpp

ResourceAssignment_Firstfit.o: ResourceAssignment_Firstfit.cpp ResourceAssignment_Firstfit.h Event.h EventQueue.h RoutingTable.h ModulationFormats.h
	$(CC) $(CPPSTD) $(DEBUGFLAGS) $(INCLUDES) $(LDFLAGS) $(LIBS) $(CFLAGS) ResourceAssignment_Firstfit.cpp

RandomVariable.o: RandomVariable.cpp RandomVariable.h 
	$(CC) $(CPPSTD) $(DEBUGFLAGS) $(INCLUDES) $(LDFLAGS) $(LIBS) $(CFLAGS) RandomVariable.cpp

Topology.o: Topology.cpp Topology.h Network.h
	$(CC) $(CPPSTD) $(DEBUGFLAGS) $(INCLUDES) $(LDFLAGS) $(LIBS) $(CFLAGS) Topology.cpp 

ModulationFormats.o: ModulationFormats.cpp ModulationFormats.h Event.h Network.h
	$(CC) $(CPPSTD) $(DEBUGFLAGS) $(INCLUDES) $(LDFLAGS) $(LIBS) $(CFLAGS) ModulationFormats.cpp

main.o: main.cpp
	$(CC) $(CPPSTD) $(DEBUGFLAGS) $(INCLUDES) $(LDFLAGS) $(LIBS) $(CFLAGS) main.cpp

.PHONY: clean
clean:
	rm $(CLEANFILES)

