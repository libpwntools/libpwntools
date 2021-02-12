#include <unistd.h>
#include <libpwntools/io.h>
#include <libpwntools/utils.h>
#include <iostream>
#include <thread>
#include <libpwntools/logger.h>

pwn::IO::IO() {};
pwn::IO::~IO() {
    std::cout << this->buffer;
    this->buffer.clear();
};

void pwn::IO::set_debug(bool mode) {
    this->debug = mode;
}

std::string pwn::IO::recvuntil(const std::string &buf) {
    std::string s;
    while (!pwn::ends_with(s, buf))
        s += this->recv(1);
    return s;
}

size_t pwn::IO::sendline(const std::string &buf) {
    return this->send(buf + "\n");
}

std::string pwn::IO::recvline() {
    return this->recvuntil("\n");
}

size_t pwn::IO::sendafter(const std::string &rcv, const std::string &data) {
    this->recvuntil(rcv);
    return this->send(data);
}

size_t pwn::IO::sendlineafter(const std::string &rcv, const std::string &data) {
    return this->sendafter(rcv, data+"\n");
}

std::string pwn::IO::recvn(size_t len) {
    std::string buf;
    size_t size_left = len;
    while (buf.length() != len) {
        buf += this->recv(size_left);
        size_left = len - buf.length();
    }
    return buf;
};

std::string pwn::IO::recv(size_t len) { // experimental
    size_t buffer_length = this->buffer.length();
    std::string tmp;
    if(len >= 1024 && !buffer_length) {
        tmp = this->recv_raw(len);
        goto ret;
    }

    if(buffer_length >= len) {
        tmp = this->buffer.substr(0, len);
        if(buffer_length > len)
            this->buffer = this->buffer.substr(len);
        else
            this->buffer.clear();
        goto ret;
    }

    if(buffer_length) {
        this->buffer += this->recv_raw(len-buffer_length);
        tmp = this->buffer;
        this->buffer.clear();
        goto ret;
    }

    this->buffer = this->recv_raw(1024);
    buffer_length = this->buffer.length();
    tmp = this->buffer.substr(0, std::min(buffer_length, len));
    if(tmp.length() == buffer_length)
        this->buffer.clear();
    else
        this->buffer = this->buffer.substr(len);

    ret:
    if(this->debug) {
        std::cout << "(Recv)\n";
        pwn::hexdump(tmp);
    }
    return tmp;
}

std::string pwn::IO::recv_raw(size_t len) { // dummy
    std::cout << "This should never be called\n";
    exit(0);
    return "";
}

size_t pwn::IO::send(const std::string&) { // dummy
    std::cout << "This should never be called\n";
    exit(0);
    return 0;
}

void pwn::IO::close() { // dummy
    std::cout << "This should never be called\n";
    exit(0);
}


void pwn::IO::interactive() {
    std::cout.setf(std::ios::unitbuf);
    std::cin.setf(std::ios::unitbuf);

    pwn::log::info("Switching to interactive mode");
    std::thread t1(
    [&]() -> void {
        std::string s;
        std::cout << this->buffer;
        this->buffer.clear();

        while(true)
            std::cout << this->recv_raw(1024);
    });

    usleep(1500);
    std::string inp;
    while(true) {
        std::cout << "$ ";
        getline(std::cin ,inp);
        this->sendline(inp);
        usleep(250000);
    }
}
