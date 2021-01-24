#include "pwntools.h"
#include <vector>
#include <iostream>
#include <string>
#include <stdint.h>

int main() {
    std::cout << pack::flat(0xdeadbeef,0x41414141, 0xbeefdeadL, "asdf", 0x42L);
}
