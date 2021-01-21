#include <string>
#include <iostream>
#include "pwntools.h"

int main(void) {
 Process io("/bin/cat");
 io.send("%p%p%p%p\n");
 std::string S = io.recvline();
 std::cout << S ;
 io.interactive();
}
