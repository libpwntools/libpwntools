#include <pwntools>
#include <iostream>

int main(void) {
    fmtstr_payload p(6);
    p[0x404018] = 0x4142434445464748;
    p[0x404020] = 0xdeadbeefcafebabe;
    std::cout << p.build();
}
