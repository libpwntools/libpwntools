all:
	g++ utils.cc Socket.cc remote.cc process.cc elf.cc main.cc -o ../bin/main -pthread -std=c++17
shared:
	g++ utils.cc Socket.cc remote.cc process.cc elf.cc -o ../bin/libpwntools.so -pthread -shared -fPIC -g -std=c++17
