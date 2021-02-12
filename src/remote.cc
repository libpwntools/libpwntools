#include <libpwntools/remote.h>
#include <libpwntools/logger.h>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#endif
#include <cassert>
#include <thread>

#ifdef __linux__
int send_wrapper(int f, const char * buf, size_t len, int z) {
    return send(f, buf, len, z);
}
int recv_wrapper(int f, char* buf, size_t len, int z) {
    return recv(f, buf, len, z);
}
#elif _WIN32
int send_wrapper(SOCKET f, const char* buf, size_t len, int z) {
    return send(f, buf, len, z);
}

int recv_wrapper(SOCKET f, char* buf, size_t len, int z) {
    return recv(f, buf, len, z);
}
#endif


pwn::Remote::Remote() {}

pwn::Remote::~Remote() {
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
    if (res != 0) {
        pwn::log::error("WSAStartup failed with error: " + std::to_string(iResult));
        exit(1);
    }
#endif
    hostent * record = gethostbyname(ip.c_str());
    if(record == nullptr) {
        std::cerr << ip << " is unavailable\n";
        exit(-1);
    }
	serv_addr.sin_addr = *(in_addr *)record->h_addr;

    if((this->fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Error while creating socket\n";
        exit(-1);
    }

    if (connect(this->fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection failed\n";
        exit(-1);
    }
    this->debug = false;
}

std::string pwn::Remote::recv_raw(size_t len) {
	char * buf = (char *)malloc(len);
#ifdef __linux__
	len = recv_wrapper(this->fd, buf, len, 0);
#elif _WIN32
    len = recv_wrapper(this->fd, buf, len, 0);
#endif
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
#ifdef _WIN32
    res = send_wrapper(this->fd, data.c_str(), data.size(), 0);
    if (res == SOCKET_ERROR) {
        std::cout << "send failed with error : " << WSAGetLastError() << std::endl;
        closesocket(this->fd);
        WSACleanup();
        exit(1);
    }
#elif __linux__
    res = send_wrapper(this->fd, data.c_str(), data.size(), 0);
    if (res < 0) {
        std::cout << "send failed" << std::endl;
        this->close();
        exit(1);
    }
#endif
    return res;
}

#ifdef __linux__
void pwn::Remote::shutdown(const std::string& h) {
    int how;

    if(h == "send") {
        how = SHUT_WR;
    } else if (h == "recv") {
        how = SHUT_RD;
    } else {
        std::cout << "Only send / recv allowed: " << std::endl;
        exit(1);
    }

    if (::shutdown(this->fd, how) < 0) {
        std::cout << "Shutdown err" << std::endl;
        exit(1);
    }
}
#endif
void pwn::Remote::close() {
#ifdef __linux__
	::close(this->fd);
#elif _WIN32
    closesocket(this->fd);
#endif
}
