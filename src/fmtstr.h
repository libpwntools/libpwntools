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
#include <functional>
#include <string>
#include <utility>
#include <vector>

#ifndef FMTSTR_H
#define FMTSTR_H
namespace pwn {
    class fmtstr_payload {
      public:
        fmtstr_payload();
        ~fmtstr_payload();
        fmtstr_payload(uint32_t);
        fmtstr_payload(uint32_t, uint32_t);
        void do_write(uint64_t, uint64_t);
        void do_single_write(uint64_t, uint8_t);
        void set_bytes_written(size_t);
        int64_t find_offset(std::function<std::string(const std::string &)>);
        std::string build();
        uint64_t &operator[](uint64_t addr);

      private:
        uint32_t offset;
        size_t bytes_written;
        size_t padding;
        std::unordered_map<uint64_t, uint64_t> writes;
        std::vector<std::pair<uint64_t, uint64_t>> list;
        uint64_t get_write_size();
    };
}; // namespace pwn
#endif
