#pragma once
#include <string>
#include <cassert>
#include <unistd.h>

#ifndef ELF_H
#define ELF_H
class ELF {
    public:
        ELF(const std::string&);
        int64_t find_str_offset(const std::string&);
    private:
        std::string file;
};
#endif
