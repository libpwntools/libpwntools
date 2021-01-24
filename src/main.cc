#include <pwntools>
#include <iostream>

int main(void) {
    Process io("/bin/cat");
    io.interactive();
}
