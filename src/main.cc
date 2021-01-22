#include "pwntools.h"
#include <vector>
#include <iostream>
#include <string>
#include <stdint.h>

int main() {
    Process p("/bin/cat");
    p.set_debug(true);
    p.interactive();
}
