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

struct packed_64 {
  char bytes[8];
}__attribute__((packed));

struct packed_32 {
  char bytes[4];
}__attribute__((packed));

class Char {

 public:
  char *bytes;
  uint64_t len;

  Char(uint64_t l) {
   this->len = l;
   this->bytes = new char[len + 1];
   if(!this->bytes) {
     std::cout << "(New): Exiting" << std::endl;
     exit(-1);
   }
  }

  Char(const char * c) {
   this->len = strlen(c);
   this->bytes = new char[this->len + 1];
   for(int i = 0; i < this->len; i++ ) { this->bytes[i] =  c[i] ; }
  }

  ~Char() { delete this->bytes; }

  char* raw() { return this->bytes; }

  char at(uint32_t index) {
   if(index >= this->len || index < 0) {
    std::cout << "(at): Invalid Index" << std::endl;
    exit(-1);
   }
   return this->bytes[index];
  }

  void put(FILE *fp) { fwrite(this->bytes, this->len, 1, fp); }

  void ljust(uint64_t size, const char fillcharacter) {
   if (size <= this->len ) {
    std::cout << "(ljust): size < base_size." << std::endl;
    exit(-1);
   }
   uint64_t new_size = size + this->len;
   char* temp_bytes = new char[new_size + 1];
   for(int i = 0; i < this->len; i++) { temp_bytes[i] = this->bytes[i]; }
   for(int i = this->len ; i < size; i++) { temp_bytes[i] = fillcharacter; }
   delete this->bytes;
   this->len = new_size;
   this->bytes = temp_bytes;
  }

  Char* sustitute(uint32_t s, uint32_t t) {
   if(s < 0 || s >= t || s >= this->len || t <= 0 || t >= this->len) {
    std::cout << "(Substitute): " << std::endl;
    exit(-1);
   }
   uint64_t required_len = t - s;
   Char* c = new Char(required_len);
   for(int i = s, k = 0 ; i < t; i ++ ) { c->bytes[k++] = this->bytes[i]; }
   return c;
  }

  void reverse() {
   Char* tmp = new Char(this->len);
   for(int i = (this->len - 1),k = 0; i >= 0; i--) { tmp->bytes[k++] = this->bytes[i]; }
   delete this->bytes;
   this->bytes = tmp->bytes;
  }
};

class General {
 public:
  Char* Add(std::vector<Char*> arguments) {
   uint64_t total_size_required {0};
   for(int i = 0; i < arguments.size(); i++) { total_size_required += arguments.at(i)->len; }
   Char* c = new Char(total_size_required);
   for(int i = 0,k = 0; i < arguments.size(); i++ ) {
    for (int j = 0; j < arguments.at(i)->len; j++ ) { c->bytes[k++] = arguments.at(i)->bytes[j]; }
   }
   return c;
  }
};

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
