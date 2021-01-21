#pragma once
using namespace std::string_literals;

class Process {
    public:
        Process();
        Process(const std::string&);
        size_t send(const std::string&);
        std::string recv(size_t len);
    private:
        int pid;
        int _stdin;
        int _stdout;
};
