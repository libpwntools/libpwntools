#pragma once
#include <string>
#include <cassert>
#include <stdexcept>

#ifndef UTILS_H
#define UTILS_H
namespace pwn {
    bool ends_with(const std::string&, const std::string&);
    std::string str_repeat(const std::string&, size_t);
    std::string str_xor(const std::string&, const std::string&);
    void hexdump_wrap(void *, size_t);
    void hexdump(std::string);
    std::string string_to_hex(const std::string&);
    int hex_value(unsigned char);
    std::string hex_to_string(const std::string&);
    std::string remove_newline(std::string&);
    std::string Hex(uint64_t);
    uint64_t Int(const std::string&, uint8_t);
    std::string random_string(size_t);
}
#endif
