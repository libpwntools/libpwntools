#pragma once
#include <string>
#include <cassert>
#include <unistd.h>

class ELF {
    public:
        ELF(const std::string&);
        int64_t find_str_offset(const std::string&);
    private:
        std::string file;
};
