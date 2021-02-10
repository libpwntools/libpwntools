#include <pwntools>
#include <iostream>
using namespace pwn;

int main(void) {
    Process p("/bin/cat");
    p.sendline("Test");
    p.interactive();
}
