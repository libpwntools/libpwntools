#pragma once
#include <libpwntools/io.h>

class Process : public IO {
    public:
        Process();
        Process(const std::string&);
        size_t send(const std::string&);
        std::string recv(size_t len);
        void gdb_attach();
        void _close();
    private:
        int pid;
        int _stdin;
        int _stdout;
};