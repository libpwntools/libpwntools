#include "pwntools.h"
#include <vector>
#include <iostream>
#include <string>
#include <stdint.h>

int main() {
    ELF libc("/lib/x86_64-linux-gnu/libc.so.6");
    printf("0x%x\n", libc.find_str_offset("/bin/sh"));
}
