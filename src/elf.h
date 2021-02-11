#pragma once
#include <string>
#include <cassert>
#include <unistd.h>
#include <vector>

#ifndef ELF_H
#define ELF_H
namespace pwn {
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
            ELF(const std::string&);
            int64_t find_str_offset(const std::string&);
            std::vector<section> parse_sections();
        private:
            std::string file;
    };
}
#endif
