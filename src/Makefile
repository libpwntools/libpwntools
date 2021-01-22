all:
	g++ utils.cc Socket.cc remote.cc process.cc elf.cc main.cc -o /tmp/main -pthread
shared:
	g++ utils.cc Socket.cc remote.cc process.cc elf.cc -o /lib/libpwn.so -pthread -shared -fPIC -g
