LIBS=-lpthread
Scheduler.exe: Scheduler.o
	g++ Scheduler.o -lpthread -o Scheduler.exe -Wall
	rm Scheduler.o
Scheduler.o:
	g++ -c Scheduler.cpp -o Scheduler.o
