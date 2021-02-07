#include <unistd.h>
#include <libpwntools/io.h>
#include <libpwntools/utils.h>
#include <iostream>
#include <thread>

IO::IO() {};

void IO::recvloop() {
    std::string s;
    while(true) {
        s = this->recv(1024);
        write(1, s.c_str(), s.length());
        s.clear();
    }
}

void IO::set_debug(bool mode) {
    this->debug = mode;
}

std::string IO::recvuntil(const std::string &buf) {
    std::string s;
    while (!ends_with(s, buf))
        s += this->recv(1);
    if(this->debug) { std::cout << "(Recv)\n"; hexdump(s) ;}
    return s;
}

size_t IO::sendline(const std::string &buf) {
    return this->send(buf + "\n");
}

std::string IO::recvline() {
    return this->recvuntil("\n");
}

size_t IO::sendafter(const std::string &rcv, const std::string &data) {
    this->recvuntil(rcv);
    return this->send(data);
}

size_t IO::sendlineafter(const std::string &rcv, const std::string &data) {
    return this->sendafter(rcv, data+"\n");
}

std::string IO::recv(size_t len) { // dummy
    std::cout << "This should never be called\n";
    exit(0);
    return "";
}

size_t IO::send(const std::string&) { // dummy
    std::cout << "This should never be called\n";
    exit(0);
    return 0;
}

void IO::interactive() {
    std::cout.setf(std::ios::unitbuf);
    std::cin.setf(std::ios::unitbuf);

    std::thread t1(&IO::recvloop, this);
    usleep(1500);

    std::string inp;
    while(true) {
        std::cout << "$ ";
        getline(std::cin ,inp);
        if(this->debug) {std::cout << "(Send)\n"; hexdump(inp); }
        this->sendline(inp);
        usleep(250000);
    }
}
