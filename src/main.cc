#include <pwntools>
#include <iostream>

int main(void) {
    Process p("/bin/cat");
    p.sendline("Test");
    p.interactive();
}
