#include <pwntools>
#include <iostream>
using namespace pwn;

int main(void) {
    hexdump(flat(1, 2, 3L, -4, "test"));
}
