#include <iostream>
#include <pwntools>
using namespace pwn;

int main(void) {
    hexdump(flat(1, 2, -3, 4L, "test"));
}
