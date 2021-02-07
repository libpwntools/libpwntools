build:
	g++ io.cc fmtstr.cc utils.cc Socket.cc remote.cc process.cc elf.cc -o ../bin/libpwntools.so -pthread -shared -fpic -s -std=c++17
	g++ -o ../bin/main -pthread -std=c++17 main.cc -lpwntools -s
static:
	g++ *.cc -o ../bin/main -pthread -s -std=c++17 -static
	upx ../bin/main
