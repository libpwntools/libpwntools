build:
	g++ pwntools.cc io.cc logger.cc fmtstr.cc utils.cc remote.cc process.cc elf.cc -o /tmp/libpwntools.so -pthread -shared -fPIC -s -std=c++17
	g++ -o /tmp/main -pthread -std=c++17 main.cpp -lpwntools -s
static:
	g++ *.cc main.cpp -o /tmp/main -pthread -s -std=c++17 -static
	upx /tmp/main
