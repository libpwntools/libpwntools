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


namespace neolib {

        template<class Elem, class Traits>
        inline void hex_dump(const void* aData, std::size_t aLength, std::basic_ostream<Elem, Traits>& aStream, std::size_t aWidth = 16)
        {
                const char* const start = static_cast<const char*>(aData);
                const char* const end = start + aLength;
                const char* line = start;
                while (line != end)
                {
                        aStream.width(4);
                        aStream.fill('0');
                        aStream << std::hex << line - start << " : ";
                        std::size_t lineLength = std::min(aWidth, static_cast<std::size_t>(end - line));
                        for (std::size_t pass = 1; pass <= 2; ++pass)
                        {
                                for (const char* next = line; next != end && next != line + aWidth; ++next)
                                {
                                        char ch = *next;
                                        switch(pass)
                                        {
                                        case 1:
                                                aStream << (ch < 32 ? '.' : ch);
                                                break;
                                        case 2:
                                                if (next != line)
                                                        aStream << " ";
                                                aStream.width(2);
                                                aStream.fill('0');
                                                aStream << std::hex << std::uppercase << static_cast<int>(static_cast<unsigned char>(ch));
                                                break;
                                        }
                                }
                                if (pass == 1 && lineLength != aWidth)
                                        aStream << std::string(aWidth - lineLength, ' ');
                                aStream << " ";
                        }
                        aStream << std::endl;
                        line = line + lineLength;
                }
        }

        void hexdump(std::string data) {
         hex_dump(data.c_str(),data.size(),std::cout);
        }

        void pause() {
            char pause_buffer[0x100];
            std::cout << "(pause) Press enter to release." << std::endl;
            fgets(pause_buffer,0x2,stdin);
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
      std::string s((char *)fp, sizeof(FILE));
      return s;
  }
};

#endif
#include "process.h"
#include "remote.h"
#include "utils.h"
