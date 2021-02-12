build:
	mv src/main.cc src/.main.tmp
	g++ src/*.cc -o bin/libpwntools.so -pthread -shared -fPIC -g -std=c++17 -I`pwd`/src/
	mv src/.main.tmp src/main.cc
exploit:
	g++ -o exploit -pthread -std=c++17 exploit.cc -lpwntools
install:
	sudo ln -s -f `pwd`/src /usr/include/c++/9/libpwntools
	sudo ln -s -f `pwd`/src/pwntools /usr/include/c++/9/pwntools
	sudo ln -s -f `pwd`/bin/libpwntools.so /lib/libpwntools.so
