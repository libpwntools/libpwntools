#pragma once
#ifndef PWNTOOLS_H
#define PWNTOOLS_H
#include <iostream>
#include <string>
#include <vector>
#include <string.h>
#include <unordered_map>
#include <stdio.h>
#include <cstddef>
#include <list>
#include <cassert>
#define AMD64 64
#define I386 86

namespace neolib{
    void pause() {
        std::cout << "(pause) Press enter to release." << std::endl;
        getchar();
    }
}

namespace pack {
    std::string pack(uint64_t n) {
        return std::string((char *)&n, 8);
    }

    std::string pack(uint32_t n) {
        return std::string((char *)&n, 4);
    }

    std::string pack(int64_t n) {
        return std::string((char *)&n, 8);
    }

    std::string pack(unsigned long long n) {
        return std::string((char *)&n, 8);
    }

    std::string pack(long long n) {
        return std::string((char *)&n, 8);
    }

    std::string pack(int32_t n) {
        return std::string((char *)&n, 4);
    }

    std::string pack(const std::string &s) {
        return s;
    }

    std::string p64(uint64_t n) {
        return pack::pack(n);
    }

    std::string p32(uint32_t n) {
        return pack::pack(n);
    }

    uint64_t u64(const std::string &buf) {
        assert(buf.length() == 8);
        return *(uint64_t *)buf.c_str();
    }

    uint32_t u32(const std::string &buf) {
        assert(buf.length() == 4);
        return *(uint32_t *)buf.c_str();
    }

    template <typename T>
    void impl_(std::string &buf, T n) {
        buf += pack::pack(n);
    }

    template <typename T>
    void flat_raw(std::string &buf, T const& arg)
    {
        pack::impl_(buf, arg);
    }

    template <typename T, typename... Ts>
    void flat_raw(std::string &buf, T const& arg, Ts&&... args) {
        pack::flat_raw(buf, arg);
        pack::flat_raw(buf, std::forward<Ts>(args)...);
    }

    template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    constexpr void impl_(std::string &buf, T const& value) {
        buf += std::string((char *)&value, sizeof(T));
    }

    template <typename T>
    std::string flat(T const &arg) {
        std::string buf;
        pack::flat_raw(buf, arg);
        return buf;
    }

    template <typename T, typename... Ts>
    std::string flat(T const& arg, Ts&&... args)
    {
        std::string buf;
        pack::flat_raw(buf, arg);
        pack::flat_raw(buf, std::forward<Ts>(args)...);
        return buf;
    }

    std::string construct_file(FILE *fp) {
        return std::string((char *)fp, sizeof(FILE));
    }
};

#endif
#include "process.h"
#include "remote.h"
#include "utils.h"
#include "elf.h"
#include "logger.h"
