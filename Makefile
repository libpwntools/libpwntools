build:
	g++ src/fmtstr.cc src/utils.cc src/Socket.cc src/remote.cc src/process.cc src/elf.cc -o bin/libpwntools.so -pthread -shared -fPIC -g -std=c++17 -I`pwd`/src/
exploit:
	g++ -o exploit -pthread -std=c++17 exploit.cc
install:
	sudo ln -s `pwd`/src /usr/include/c++/9/libpwntools
	sudo ln -s `pwd`/src/pwntools /usr/include/c++/9/pwntools
	sudo ln -s `pwd`/bin/libpwntools.so /lib/libpwntools.so
