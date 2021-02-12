build:
	g++ pwntools.cc io.cc logger.cc fmtstr.cc utils.cc remote.cc process.cc elf.cc -o /tmp/libpwntools.so -pthread -shared -fPIC -s -std=c++17
	g++ -o ../bin/main -pthread -std=c++17 main.cpp -lpwntools -s
static:
	g++ *.cc -o ../bin/main -pthread -s -std=c++17 -static
	upx ../bin/main
