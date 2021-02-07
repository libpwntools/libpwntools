#include <pwntools>
#include <iostream>

int main(void) {
    Process io("/bin/cat");
    io.sendline("QWERTYUIOP");
    for(int i=0;i<5;++i)
        std::cout << io.recv_buffered(2) << '\n';
    io.interactive();
}
