project: process.o memory_manager.o
	g++ -std=c++0x -lstdc++ process.o memory_manager.o -o project.out
process.o: process.cpp process.h
	g++ -std=c++0x -c -lstdc++ process.cpp
sceduler.o: memory_manager.cpp process.h
	g++ -std=c++0x -c -lstdc++ memory_manager.cpp
clean:
	rm *.o *.out