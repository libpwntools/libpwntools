#include <libpwntools/remote.h>
#include <cassert>
#include <thread>

size_t send_wrapper(int fd, const char * buf, size_t len, int z) {
    return send(fd, buf, len, z);
}

size_t recv_wrapper(int fd, char * buf, size_t len, int z) {
    return recv(fd, buf, len, z);
}

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

    hostent * record = gethostbyname(ip.c_str());
	if(record == NULL) {
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
	len = recv_wrapper(this->fd, buf, len, 0);
	std::string s(buf, len);
	free(buf);
	return s;
}

size_t pwn::Remote::send(const std::string &data) {
    if(this->debug) {
        std::cout << "(Send)\n";
        pwn::hexdump(data);
    }
    return send_wrapper(this->fd, data.c_str(), data.size(), 0);
}

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

void pwn::Remote::close() {
	::close(this->fd);
}
