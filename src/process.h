#pragma once

class Process {
    public:
        Process();
        Process(const std::string&);
        size_t send(const std::string&);
        std::string recv(size_t len);
        void interactive();
        size_t sendline(const std::string&);
        std::string recvuntil(const std::string&);
        std::string recvline();
        size_t sendlineafter(const std::string&, const std::string&);
        size_t sendafter(const std::string&, const std::string&);
        void gdb_attach();
        void _close();
        void set_debug(bool);
    private:
        int pid;
        int _stdin;
        int _stdout;
        bool debug;
        void recvloop();
};

void hexdump(void *, long);
