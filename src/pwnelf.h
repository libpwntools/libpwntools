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

#pragma once
#include <unistd.h>

#include <cassert>
#include <string>
#include <unordered_map>
#include <vector>

#ifndef ELF_H
#define ELF_H
namespace pwn {
    typedef struct {
        std::string symbol_name;
        std::string symbol_section;
        uint64_t symbol_value;
        uint64_t symbol_size;
        uint64_t symbol_type;
    } symbol;

    typedef struct {
        std::string section_name;
        uint64_t section_type;
        uint64_t section_offset;
        uint64_t section_address;
        uint64_t section_size;
        uint64_t section_entsize;
        uint64_t section_addr_align;
    } section;

    class ELF {
      public:
        ELF(const std::string &);
        int64_t find_str_offset(const std::string &);
        std::vector<section> parse_sections();
        std::vector<symbol> parse_symbols();
        uint64_t &operator[](const std::string &);

      private:
        std::string file;
        std::unordered_map<std::string, uint64_t> sym_map;
    };
} // namespace pwn
#endif
