#pragma once
#include <string>
#include <vector>
#include <utility>
#include <algorithm>

#ifndef FMTSTR_H
#define FMTSTR_H
class fmtstr_payload {
    public:
        fmtstr_payload(uint32_t);
        void do_write(uint64_t, uint64_t); 
        void set_bytes_written(size_t);
        std::string build();
    private:
        uint32_t offset;
        size_t bytes_written;
        size_t padding;
        std::vector<std::pair<uint64_t, uint64_t>> list;
};
#endif
