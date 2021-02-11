#include <pwntools>
#include <iostream>
using namespace pwn;

int main(void) {
    ELF e("/tmp/fmt");
    std::cout << Hex(e["main"]) << '\n';
}
