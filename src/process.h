#pragma once
using namespace std::string_literals;

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
    private:
        int pid;
        int _stdin;
        int _stdout;
        void recvloop();
};
