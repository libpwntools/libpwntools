#include <pwntools>
#include <iostream>
using namespace pwn;

int main(void) {
    ELF e("/tmp/fmt");
    auto sections = e.parse_sections();
    for(auto sec : sections)
        std::cout << sec.section_name << '\n';
}
