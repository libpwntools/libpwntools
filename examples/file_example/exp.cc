#include <pwntools>
#include <vector>
#include <iostream>

int main() {
	file f;
	f.fp->_fileno = 0x44;
	std::string fake_stream = f.construct();
	hexdump(fake_stream);
}
