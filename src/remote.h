#pragma once
#include <iostream>
#include <string>
#include <libpwntools/remote.h>
#include <libpwntools/io.h>
#include <libpwntools/utils.h>
#include <netdb.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <fstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#ifndef REMOTE_H
#define REMOTE_H
class Remote : public IO {
    private:
        std::string host;
        std::string port;
        int fd;
    public:
        Remote();
        ~Remote();
        Remote(const std::string&, uint32_t);
        std::string recv_raw(size_t);
        size_t send(const std::string&);
        void shutdown(const std::string& );
        void close();
};
#endif
