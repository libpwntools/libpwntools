#pragma once
#include <libpwntools/io.h>
#include <libpwntools/remote.h>
#include <libpwntools/utils.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <string>
#ifdef __linux__
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <unistd.h>
#elif _WIN32
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
#ifdef __linux__
        int fd;
#elif _WIN32
        WSADATA wsaData;
        SOCKET fd;
#endif
       public:
        Remote();
        ~Remote();
        Remote(const std::string&, uint32_t);
        virtual std::string recv_raw(size_t) override;
        virtual size_t send(const std::string&) override;
#ifdef __linux__
        void shutdown(const std::string&);
#endif
        virtual void close() override;
    };
}  // namespace pwn
#endif
