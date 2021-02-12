#pragma once
#ifndef PWNTOOLS_H
#define PWNTOOLS_H
#include <iostream>
#include <string>
#include <vector>
#include <string.h>
#include <unordered_map>
#include <stdio.h>
#include <cstddef>
#include <list>
#include <cassert>
#include <cstring>

namespace pwn {
    void pause();
    std::string pack(uint64_t n);
    std::string pack(uint32_t n);
    std::string pack(int64_t n);
    std::string pack(int32_t n);
    std::string pack(const std::string &s);
    std::string p64(uint64_t n);
    std::string p32(uint32_t n);
    uint64_t u64(const std::string &buf);
    uint32_t u32(const std::string &buf);

    template <typename T>
    void impl_(std::string &buf, T n) {
        buf += pwn::pack(n);
    }

    template <typename T>
    void flat_raw(std::string &buf, T const& arg)
    {
        pwn::impl_(buf, arg);
    }

    template <typename T, typename... Ts>
    void flat_raw(std::string &buf, T const& arg, Ts&&... args) {
        pwn::flat_raw(buf, arg);
        pwn::flat_raw(buf, std::forward<Ts>(args)...);
    }

    template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    constexpr void impl_(std::string &buf, T const& value) {
        buf += std::string((char *)&value, sizeof(T));
    }

    template <typename T>
    std::string flat(T const &arg) {
        std::string buf;
        pwn::flat_raw(buf, arg);
        return buf;
    }

    template <typename T, typename... Ts>
    std::string flat(T const& arg, Ts&&... args)
    {
        std::string buf;
        pwn::flat_raw(buf, arg);
        pwn::flat_raw(buf, std::forward<Ts>(args)...);
        return buf;
    }

    #ifdef __linux__
    class file {
        public:
            FILE* fp;
    		file();
    		std::string construct();
            ~file();
    };

    class SigReturnFrame {
        public:
            std::string arch;
            std::string kernel;
            uint32_t gs_32;
            uint32_t fs_32;
            uint32_t es_32;
            uint32_t ds_32;
            uint32_t edi_32;
            uint32_t esi_32;
            uint32_t ebp_32;
            uint32_t esp_32;
            uint32_t ebx_32;
            uint32_t edx_32;
            uint32_t ecx_32;
            uint32_t eax_32;
            uint32_t trapno_32;
            uint32_t err_32;
            uint32_t eip_32;
            uint32_t cs_32;
            uint32_t eflags_32;
            uint32_t esp_at_signal_32;
            uint32_t ss_32;
            uint32_t fpstate_32;

            uint64_t uc_flags;
            uint64_t uc;
            uint64_t uc_stack_ss_sp;
            uint64_t uc_stack_ss_flags;
            uint64_t uc_stack_ss_size;
            uint64_t r8;
            uint64_t r9;
            uint64_t r10;
            uint64_t r11;
            uint64_t r12;
            uint64_t r13;
            uint64_t r14;
            uint64_t r15;
            uint64_t rdi;
            uint64_t rsi;
            uint64_t rbp;
            uint64_t rbx;
            uint64_t rdx;
            uint64_t rax;
            uint64_t rcx;
            uint64_t rsp;
            uint64_t rip;
            uint64_t eflags_64;
            uint64_t csgsfs;
            uint64_t err_64;
            uint64_t trapno_64;
            uint64_t oldmask_64;
            uint64_t cr2_64;
            uint64_t fpstate_64;
            uint64_t reserved;
            uint64_t sigmask;
            SigReturnFrame(std::string kern, std::string file_arch);
            std::string construct_frame();
    };
    #endif
};
/*
#include <libpwntools/elf.h>
#include <libpwntools/process.h>
#include <libpwntools/remote.h>
#include <libpwntools/utils.h>
#include <libpwntools/logger.h>
#include <libpwntools/fmtstr.h>
*/

#include "process.h"
#include "remote.h"
#include "utils.h"
#include "logger.h"
#include "fmtstr.h"
#endif