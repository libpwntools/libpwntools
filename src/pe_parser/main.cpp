#include "pe_parser/pe.h"
#include <iostream>

int main() {
    PE dll("./ida64.dll");
    uint64_t addr = dll["MD5Final"];
    std::cout << "MD5Final: 0x" << std::hex << addr return 0;
}
