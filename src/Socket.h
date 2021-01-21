//
//  Socket.h
//  SocketServer
//
//  Created by Kay Makowsky on 14.06.16.
//  Copyright © 2016 Kay Makowsky. All rights reserved.
//

#pragma once
#ifndef SOCKET_H
#define SOCKET_H
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <vector>
#include <sys/un.h>
#include <errno.h>

class Socket
{
public:
    int sock;
    std::string address;
    std::string port;
    struct addrinfo address_info;
    Socket();
    Socket(int domain,int type,int protocol);
    int bind(std::string ip, std::string port);
    int connect(std::string ip, std::string port);
    int listen(int max_queue);
    Socket* accept();
    int socket_write(std::string msg);
    int socket_read(std::string &buf,int len);
    int socket_safe_read(std::string &buf,int len,int seconds);
    int socket_writeTo(std::string msg, std::string ip, std::string port);
    int socket_readFrom(std::string &buf, int len, std::string ip, std::string port);
    int socket_set_opt(int level, int optname, void* optval);
    int socket_get_opt(int level, int optname, void* optval);
    int set_blocking();
    int set_non_blocking();
    int socket_shutdown(int how);
    void close();
    static int select(std::vector<Socket> *reads, std::vector<Socket> *writes, std::vector<Socket> *exceptions, int seconds);
    static std::string ipFromHostName(std::string hostname);
};
#endif
