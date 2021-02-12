#pragma once
#include <string>

#ifndef IO_H
#define IO_H
namespace pwn {
    class IO {
        public:
            IO();
            ~IO();
            virtual size_t send(const std::string&);
            virtual std::string recv_raw(size_t);
            std::string recv(size_t);
            void interactive();
            size_t sendline(const std::string&);
            std::string recvuntil(const std::string&);
            std::string recvline();
            std::string recvn(size_t);
            size_t sendlineafter(const std::string&, const std::string&);
            size_t sendafter(const std::string&, const std::string&);
            void set_debug(bool);
            virtual void close();
        protected:
            bool debug;
        private:
            std::string buffer;
    };
}
#endif
