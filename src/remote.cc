#ifdef __linux__
#include <libpwntools/logger.h>
#include <libpwntools/remote.h>
#elif _WIN32
#include "logger.h"
#include "remote.h"
#define WIN32_LEAN_AND_MEAN
#endif
#include <cassert>
#include <thread>

int send_wrapper(sock f, const char *buf, size_t len, int z) {
    return send(f, buf, len, z);
}

int recv_wrapper(sock f, char *buf, size_t len, int z) {
    return recv(f, buf, len, z);
}

pwn::Remote::Remote() {
}

pwn::Remote::~Remote() {
    if (this->status == running)
        return;
    this->close();
}

pwn::Remote::Remote(const std::string &ip, uint32_t port_number) {
    this->host = ip;
    this->port = std::to_string(port_number);

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_number);

#ifdef _WIN32
    int res = 0;
    res = WSAStartup(MAKEWORD(2, 2), &this->wsaData);
    if (res != 0)
        pwn::abort("WSAStartup failed with error: " + std::to_string(res));
#endif
    hostent *record = gethostbyname(ip.c_str());
    if (record == nullptr)
        pwn::abort(ip + " is unavailable");
    serv_addr.sin_addr = *(in_addr *)record->h_addr;

    if ((this->fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        pwn::abort("Error while creating socket");

    if (connect(this->fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        pwn::abort("Connection failed");

    this->debug = false;
    this->status = running;
}

std::string pwn::Remote::recv_raw(size_t len) {
    char *buf = (char *)malloc(len);
    len = recv_wrapper(this->fd, buf, len, 0);
    std::string s(buf, len);
    free(buf);
    return s;
}

size_t pwn::Remote::send(const std::string &data) {
    if (this->debug) {
        std::cout << "(Send)\n";
        pwn::hexdump(data);
    }
    size_t res;
    res = send_wrapper(this->fd, data.c_str(), data.size(), 0);
#ifdef _WIN32
    if (res == SOCKET_ERROR) {
        pwn::log::error("send failed with error : " +
                        std::to_string(WSAGetLastError()));
        closesocket(this->fd);
        WSACleanup();
        exit(1);
    }
#elif __linux__
    if (res < 0) {
        pwn::log::error("send failed");
        this->close();
        exit(1);
    }
#endif
    return res;
}

#ifdef __linux__
void pwn::Remote::shutdown(const std::string &h) {
    int how;

    if (h == "send")
        how = SHUT_WR;
    else if (h == "recv")
        how = SHUT_RD;
    else
        pwn::abort("Only send / recv allowed");

    if (::shutdown(this->fd, how) < 0)
        pwn::abort("Shutdown err");
}
#endif
void pwn::Remote::close() {
#ifdef __linux__
    ::close(this->fd);
#elif _WIN32
    closesocket(this->fd);
#endif
    this->status = closed;
}
