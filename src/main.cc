#include <string>
#include <iostream>
#include "pwntools.h"

int main(void) {
 using neolib::hexdump;
 Process io("../../test_ctf/christmas-carol2");

 io.sendlineafter("> ","a");
 io.interactive();
}
