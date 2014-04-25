project: process.o memory_manager.o
	g++ process.o memory_manager.o -o project.out
process.o: process.cpp process.h
	g++ -c process.cpp
sceduler.o: memory_manager.cpp process.h
	g++ -c memory_manager.cpp
clean:
	rm *.o *.out