#include "pwntools"
#include <iostream>
using namespace pwn;

int main(void) {
    hexdump(flat(( (uint64_t)1,(uint64_t) 2, (uint64_t) 3,(int64_t) -4, "test")) );
}
