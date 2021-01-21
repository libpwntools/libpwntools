#include "remote.h"
#include "utils.h"
#include <cassert>
#include <thread>

size_t send_wrapper(int fd, const char * buf, size_t len, int z) {
    return send(fd, buf, len, z);
}

size_t recv_wrapper(int fd, char * buf, size_t len, int z) {
    return recv(fd, buf, len, z);
}

Remote::Remote(const std::string &ip, uint32_t port_number) {
    this->host = ip;
    this->port = std::to_string(port_number);
    this->sock = new Socket(AF_INET,SOCK_STREAM,0);

    if(!this->sock) {
        std::cerr << "Connection Failed " << std::endl;
        exit(-1);
    }

    sock->connect(this->host, this->port);
    reads.push_back(*this->sock);
}

std::string Remote::recv(size_t len) {
	char * buf = (char *)malloc(len);
	len = recv_wrapper(sock->sock, buf, len, 0);
	std::string s(buf, len);
	free(buf);
	return s;
}


void Remote::recvloop() {
    std::string s;
    while(true) {
        s = this->recv(1024);
        write(1, s.c_str(), s.length());
        s.clear();
    }
}

std::string Remote::recvuntil(const std::string &buf) {
    std::string s;
    while (!ends_with(s, buf))
        s += this->recv(1);
    return s;
}

size_t Remote::sendline(const std::string &buf) {
    return this->send(buf + "\n");
}

std::string Remote::recvline() {
    return this->recvuntil("\n");
}

size_t Remote::send(const std::string &data) {
    return send_wrapper(sock->sock,data.c_str(),data.size(),0);
}

size_t Remote::sendafter(const std::string &rcv, const std::string &data) {
    this->recvuntil(rcv);
    return this->send(data);
}

size_t Remote::sendlineafter(const std::string &rcv, const std::string &data) {
    return this->sendafter(rcv, data+"\n");
}

void Remote::interactive() {
    std::cout.setf(std::ios::unitbuf);
    std::cin.setf(std::ios::unitbuf);

    std::thread t1(&Remote::recvloop, this);
    usleep(1500);

    std::string inp;
    while(true) {
        std::cout << "$ ";
        getline(std::cin ,inp);
        this->sendline(inp);
        usleep(5000);
    }
}
