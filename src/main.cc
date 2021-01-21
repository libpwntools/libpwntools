#include <string>
#include <iostream>
#include "pwntools.h"

int main(void) {
 Remote io("localhost",8888);
 std::cout << io.recv(0x2);
 io.send("%p%p%p%p\n");
 std::string S = io.recvline();
 std::cout << S ;
}
