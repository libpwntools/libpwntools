#include <libpwntools/remote.h>
#include <cassert>
#include <thread>

size_t send_wrapper(int fd, const char * buf, size_t len, int z) {
    return send(fd, buf, len, z);
}

size_t recv_wrapper(int fd, char * buf, size_t len, int z) {
    return recv(fd, buf, len, z);
}

Remote::Remote() {}

Remote::Remote(const std::string &ip, uint32_t port_number) {
    this->host = ip;
    this->port = std::to_string(port_number);
    this->sock = new Socket(AF_INET,SOCK_STREAM,0);
    this->debug = false;
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
    if(this->debug && len > 1) {std::cout << "(Recv)\n"; hexdump(s) ;}
	return s;
}

size_t Remote::send(const std::string &data) {
    if(this->debug) {std::cout << "(Send)\n"; hexdump(data); }
    return send_wrapper(sock->sock,data.c_str(),data.size(),0);
}

void Remote::shutdown(const std::string& h) {
		int how {0};
    if(h == "send") {
			how = SHUT_WR;
		}
		else if (h == "recv") {
			how = SHUT_RD;
		}
		else {
			std::cout << "Only send / recv allowed: " << std::endl;
			exit(1);
		}
		if (::shutdown(sock->sock,how) < 0) {
			std::cout << "Shutdown err" << std::endl;
			exit(1);
		}
		return ;
}

void Remote::close() {
	::close(sock->sock);
}
