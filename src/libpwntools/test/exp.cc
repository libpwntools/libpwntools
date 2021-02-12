#include <pwntools.h>
#include <iostream>

int main() {

	using namespace pwn;
	Process io("/bin/cat");
	io.interactive();
	return 0;
}
