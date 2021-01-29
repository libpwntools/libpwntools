#include <pwntools>
#include <vector>
#include <iostream>

int main() {
	SigReturnFrame frame("amd64","amd64");

	frame.rbp = 0x41414141;
	frame.r9 = 0x77;

	std::string fake_frame_bytes = frame.construct_frame();
	hexdump(fake_frame_bytes);
	return 0;
}
