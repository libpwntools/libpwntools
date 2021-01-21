#pragma once
#include "Socket.h"
#include <string>

class Remote {
 private:
  std::string host;
  std::string port;
  Socket* sock;
  std::vector<Socket> reads;

 public:
  Remote(const std::string&, uint32_t);
  std::string recv(size_t);
  std::string recvline();
  std::string recvuntil(const std::string&);
  size_t send(const std::string&);
  void recvloop();
  size_t sendafter(const std::string&,const std::string&);
  size_t sendlineafter(const std::string&,const std::string&);
  void interactive();
  size_t sendline(const std::string&);
};
