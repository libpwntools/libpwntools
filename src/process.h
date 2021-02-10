#pragma once
#include <libpwntools/io.h>

#ifndef PROCESS_H
#define PROCESS_H
class Process : public IO {
    public:
        Process();
        ~Process();
        Process(const std::string&);
        size_t send(const std::string&);
        std::string recv_raw(size_t len);
        void gdb_attach();
        void close();
    private:
        int pid;
        int _stdin;
        int _stdout;
};
#endif
