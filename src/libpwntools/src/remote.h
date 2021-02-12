#pragma once
#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <libpwntools/remote.h>
#include <libpwntools/io.h>
#include <libpwntools/utils.h>
#ifdef __linux__
#include <netdb.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#elif _WIN32
#include <windows.h>
#include <winsock.h>
#pragma comment (lib, "Ws2_32.lib")
#endif

#ifndef REMOTE_H
#define REMOTE_H
namespace pwn {
    class Remote : public pwn::IO {
        private:
            std::string host;
            std::string port;
#ifdef __linux__
            int ConnectSocket{ 0 };
#elif _WIN32
            WSADATA wsaData;
            SOCKET ConnectSocket = INVALID_SOCKET;
#endif
        public:
            Remote();
            ~Remote();
            Remote(const std::string&, uint32_t);
            virtual std::string recv_raw(size_t) override;
            virtual size_t send(const std::string&) override;
#ifdef __linux__
            void shutdown(const std::string& );
#endif
            virtual void close() override;
    };
}
#endif
