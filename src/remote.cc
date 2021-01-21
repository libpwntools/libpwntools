#include "remote.h"
#include <cassert>

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

std::string Remote::recvn(uint32_t n) {
    std::string buffer;
    char tmp[n + 1];
    memset(tmp,0,sizeof(tmp));
    uint32_t total_recieved {0};
    uint32_t remaining_bytes = n;
    int status;
    
    while(total_recieved != n) {
        status = (int) recv(sock->sock,tmp,remaining_bytes,0);
        if(!status) break;
        else if ( status < 0 ) { std::cerr << "Recvline: " << std::endl ; exit(-1); }
        total_recieved += status;
        remaining_bytes -= total_recieved;
        buffer = std::string(tmp,total_recieved);
        if (!remaining_bytes) return buffer;
    }

    return buffer;
}

std::string Remote::recvline() {
    std::string buffer;
    uint32_t total_recieved {0};
    char temp;
    int status;

    while(temp != '\n') {
        status = (int)recv(sock->sock,&temp,1,0);
        if(status == 0) continue;
        if (status < 0) { std::cerr << "(Recv) " << std::endl ; exit(-1); }
        buffer += temp;
    }

    return buffer;
}

void Remote::Send(const std::string &data) {
    assert(send(sock->sock,data.c_str(),data.size(),0) >= 0);
}
