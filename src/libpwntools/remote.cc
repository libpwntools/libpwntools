#include "remote.h"
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
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port_number);

#ifdef _WIN32
    int iResult{ 0 };
    iResult = WSAStartup(MAKEWORD(2, 2), &this->wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed with error: " << iResult << std::endl;
        exit(1);
    }
#endif
    hostent* record = gethostbyname(ip.c_str());
    if (record == NULL) {
        std::cerr << ip << " is unavailable\n";
        exit(-1);
    }
	server.sin_addr = *(in_addr *)record->h_addr;

    if ((this->ConnectSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Error while Createing Socket" << std::endl;
        exit(-1);
    }

    if (connect(this->ConnectSocket, (struct sockaddr *)&server, sizeof(server)) < 0) {
        std::cerr << "Connection failed\n";
        exit(-1);
    }
    this->debug = false;

}

std::string pwn::Remote::recv_raw(size_t len) {
	char * buf = (char *)malloc(len);
#ifdef __linux__
	len = recv_wrapper(this->ConnectSocket, buf, len, 0);
#elif _WIN32
    len = recv_wrapper(this->ConnectSocket, buf, len, 0);
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
    int iResult{ 0 };
#ifdef _WIN32
    iResult = send_wrapper(this->ConnectSocket, data.c_str(),(size_t) data.size(), 0);
    if (iResult == SOCKET_ERROR) {
        std::cout << "send failed with error : " << WSAGetLastError() << std::endl;
        closesocket(this->ConnectSocket);
        WSACleanup();
        exit(1);
    }
#elif __linux__
    iResult = send_wrapper(this->ConnectSocket, data.c_str(),(size_t) data.size(), 0);
    if (iResult < 0) {
        std::cout << "send failed" << std::endl;
        this->close();
        exit(1);
    }
#endif
    return (size_t )iResult;
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

    if (::shutdown(this->ConnectSocket, how) < 0) {
        std::cout << "Shutdown err" << std::endl;
        exit(1);
    }
}
#endif
void pwn::Remote::close() {
#ifdef __linux__
	::close(this->ConnectSocket);
#elif _WIN32
    closesocket(this->ConnectSocket);
#endif
}
