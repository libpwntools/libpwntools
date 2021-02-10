#include <pwntools>
#include <iostream>
using namespace pwn;

int main(void) {
    Process p("/bin/cat");
    p.sendline("Test");
    hexdump(flat(1u, 2LL, "test", -4, 5));
    p.interactive();
}
