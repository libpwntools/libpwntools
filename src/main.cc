#include <pwntools>
#include <iostream>

int main(void) {
    Process io("/bin/cat");
    io.sendline("ASDFGHJKL);
    uint64_t leak = pack::u64(io.recv(8));
    log::success("Leak at: "+Hex(leak));
    io.interactive();
}
