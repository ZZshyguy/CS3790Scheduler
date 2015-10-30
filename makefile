Scheduler : prog.o writer.o
	g++ prog.o -o Scheduler
	g++ writer.o -o writer


prog.o : prog.cpp
	g++ -c prog.cpp

writer.o : writer.cpp
	g++ -c writer.cpp
