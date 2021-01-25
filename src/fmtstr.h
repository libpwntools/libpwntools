#pragma once
#include <string>
#include <vector>
#include <utility>

class fmtstr_payload {
    public:
        fmtstr_payload(uint32_t);
        void do_write(uint64_t, uint64_t); 
        void set_bytes_written(size_t);
        std::string gen_payload();
    private:
        uint32_t offset;
        size_t bytes_written;
        std::vector<std::pair<uint64_t, uint64_t>> list;
};
