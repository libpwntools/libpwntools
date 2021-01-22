#include "pwntools.h"
#include <vector>
#include <iostream>
#include <string>
#include <stdint.h>

int main() {
    neolib::hexdump(str_xor("test", "AAAAAAAAA"));
}
