#pragma once
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
};

class Remote {

 public:
  const char* host_ip;
  uint32_t port;

  Remote(const char* ip,uint32_t p) {
   this->host_ip = ip;
   this->port = p;
  }
};
