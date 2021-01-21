#include <string>
#include <iostream>
#include "pwntools.h"

int main(void) {
 Process io("/bin/cat");
 io.send("%p%p%p%p\n");
 std::string S = io.recvline();
 std::cout << S ;
 neolib::hex_dump(S.c_str(),S.size(),std::cout);
 io.interactive();
}
