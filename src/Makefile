build:
	g++ utils.cc Socket.cc remote.cc process.cc elf.cc -o ../bin/libpwntools.so -pthread -shared -fpic -g -std=c++17
	g++ -o ../bin/main -pthread -std=c++17 main.cc -lpwntools -s
static:
	g++ utils.cc Socket.cc remote.cc process.cc elf.cc -o ../bin/main -pthread -shared -fpic -g -std=c++17 -s
