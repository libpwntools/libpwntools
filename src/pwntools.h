#pragma once
#ifndef PWNTOOLS_H
#define PWNTOOLS_H
#include <iostream>
#include <string>
#include <vector>
#include <string.h>
#include <stdint.h>
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
      std::string p64(uint64_t n) {
        return std::string((char *)&n, 8);
      }

      uint64_t u64(const std::string &buf) {
        assert(buf.length() == 8);
        return *(uint64_t *)buf.c_str();
      }

      std::string p32(uint32_t n) {
        return std::string((char *)&n, 4);
      }

      uint32_t u32(const std::string &buf) {
        assert(buf.length() == 4);
        return *(uint32_t *)buf.c_str();
      }

      std::string flat(std::vector<uint64_t> chain) {
        std::string s;
        for(int i=0; i<chain.size(); ++i)
            s += pack::p64(chain[i]);
        return s;
      }

      std::string flat(std::vector<uint32_t> chain) {
        std::string s;
        for(int i=0; i<chain.size(); ++i)
            s += pack::p32(chain[i]);
        return s;
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
