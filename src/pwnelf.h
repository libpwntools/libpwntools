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
}  // namespace pwn
#endif
