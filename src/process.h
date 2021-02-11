#pragma once
#include <libpwntools/io.h>

#ifndef PROCESS_H
#define PROCESS_H
namespace pwn {
    class Process : public pwn::IO {
        public:
            Process();
            ~Process();
            Process(const std::string&);
            virtual size_t send(const std::string&) override;
            virtual std::string recv_raw(size_t len) override;
            void gdb_attach();
            virtual void close() override;
        private:
            int pid;
            int _stdin;
            int _stdout;
    };
}
#endif
