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

#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <string>
#ifdef __linux__
#include <arpa/inet.h>
#include <libpwntools/io.h>
#include <libpwntools/remote.h>
#include <libpwntools/utils.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <unistd.h>
#elif _WIN32
#include "io.h"
#include "remote.h"
#include "utils.h"
#include <windows.h>
#include <winsock.h>
#pragma comment(lib, "Ws2_32.lib")
#endif
#ifdef __linux__
#define sock int
#else
#define sock SOCKET
#endif

#ifndef REMOTE_H
#define REMOTE_H
namespace pwn {
    class Remote : public pwn::IO {
      private:
        std::string host;
        std::string port;
        sock fd;
#ifdef _WIN32
        WSADATA wsaData;
#endif
      public:
        Remote();
        ~Remote();
        Remote(const std::string &, uint32_t);
        virtual std::string recv_raw(size_t) override;
        virtual size_t send(const std::string &) override;
#ifdef __linux__
        void shutdown(const std::string &);
#endif
        virtual void close() override;
    };
} // namespace pwn
#endif
