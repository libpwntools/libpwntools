#include <pwntools>
#include <vector>
#include <iostream>

int main() {
	file f;
	f.fp->_fileno = 0x41;
	std::string fake_stream = f.construct();
	hexdump((void *)fake_stream.c_str(),fake_stream.size());
}
