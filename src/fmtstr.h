#pragma once
#include <string>
#include <vector>
#include <utility>
#include <functional>

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
            int64_t find_offset(std::function<std::string(const std::string&)>);
            std::string build();
            uint64_t &operator[] (uint64_t addr);

        private:
            uint32_t offset;
            size_t bytes_written;
            size_t padding;
            std::unordered_map<uint64_t, uint64_t> writes;
            std::vector<std::pair<uint64_t, uint64_t>> list;
            uint64_t get_write_size();
    };
};
#endif
