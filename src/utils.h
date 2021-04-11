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
#include <cassert>
#include <stdexcept>
#include <string>

#ifndef UTILS_H
#define UTILS_H
namespace pwn {
    bool ends_with(const std::string &, const std::string &);
    std::string str_repeat(const std::string &, size_t);
    std::string str_xor(const std::string &, const std::string &);
    void hexdump_wrap(void *, size_t);
    void hexdump(std::string);
    std::string string_to_hex(const std::string &);
    int hex_value(unsigned char);
    std::string hex_to_string(const std::string &);
    std::string remove_newline(const std::string &);
    std::string Hex(uint64_t);
    uint64_t Int(const std::string &, uint8_t);
    std::string random_string(size_t);
    void abort(const std::string &);

    class check_bounds {
      public:
        check_bounds(uintptr_t _ptr, size_t _len) {
            this->ptr = _ptr;
            this->len = _len;
        }
        template <typename T> T check(const T &t) {
            assert(((uintptr_t)t) < (this->ptr + this->len));
            assert(((uintptr_t)t) >= this->ptr);
            return t;
        }

      private:
        uintptr_t ptr;
        size_t len;
    };
} // namespace pwn
#endif
