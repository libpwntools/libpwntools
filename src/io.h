#pragma once
#include <string>

#ifndef IO_H
#define IO_H
class IO {
    public:
        IO();
        ~IO();
        size_t virtual send(const std::string&);
        std::string virtual recv(size_t);
        std::string recv_buffered(size_t);
        void interactive();
        size_t sendline(const std::string&);
        std::string recvuntil(const std::string&);
        std::string recvline();
        std::string recvn(size_t);
        size_t sendlineafter(const std::string&, const std::string&);
        size_t sendafter(const std::string&, const std::string&);
        void set_debug(bool);
        void virtual close();
    protected:
        bool debug;
        void recvloop();
        std::string buffer;
};
#endif
