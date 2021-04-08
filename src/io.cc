/*
BSD 2-Clause License

Copyright (c) 2021, libpwntools
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifdef __linux__
#include <libpwntools/io.h>
#include <libpwntools/logger.h>
#include <libpwntools/utils.h>
#include <unistd.h>
#elif _WIN32
#define NOMINMAX
#include "io.h"
#include "logger.h"
#include "utils.h"
#include <windows.h>
#endif

#include <iostream>
#include <thread>

pwn::IO::IO(){};
pwn::IO::~IO() {
    this->buffer.clear();
};

void pwn::IO::set_debug(bool mode) {
    this->debug = mode;
}

std::string pwn::IO::recvuntil(const std::string &buf) {
    std::string s;
    bool _debug = false;
    if (this->debug)
        std::swap(this->debug, _debug);
    while (!pwn::ends_with(s, buf))
        s += this->recv(1);
    if (_debug) {
        std::cout << "(Recv)\n";
        this->debug = true;
        pwn::hexdump(s);
    }
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
    return this->sendafter(rcv, data + "\n");
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
    if (len >= 1024 && !buffer_length) {
        tmp = this->recv_raw(len);
        goto ret;
    }

    if (buffer_length >= len) {
        tmp = this->buffer.substr(0, len);
        if (buffer_length > len)
            this->buffer = this->buffer.substr(len);
        else
            this->buffer.clear();
        goto ret;
    }

    if (buffer_length) {
        this->buffer += this->recv_raw(len - buffer_length);
        tmp = this->buffer;
        this->buffer.clear();
        goto ret;
    }

    this->buffer = this->recv_raw(1024);
    buffer_length = this->buffer.length();
    tmp = this->buffer.substr(0, std::min(buffer_length, len));

    if (tmp.length() == buffer_length)
        this->buffer.clear();
    else
        this->buffer = this->buffer.substr(len);

ret:
    if (this->debug) {
        std::cout << "(Recv)\n";
        pwn::hexdump(tmp);
    }
    return tmp;
}

void pwn::IO::interactive() {
    std::cout.setf(std::ios::unitbuf);
    std::cin.setf(std::ios::unitbuf);

    pwn::log::info("Switching to interactive mode");
    std::thread t1([&]() -> void {
        std::string s;
        std::cout << this->buffer;
        this->buffer.clear();

        while (true)
            std::cout << this->recv_raw(1024);
    });
#ifdef __linux__
    usleep(1500);
#elif _WIN32
    Sleep(50);
#endif
    std::string inp;
    while (true) {
        std::cout << "$ ";
        getline(std::cin, inp);
        this->sendline(inp);
#ifdef __linux__
        usleep(250000);
#elif _WIN32
        Sleep(150);
#endif
    }
}
