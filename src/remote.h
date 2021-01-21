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
  std::string recvn(uint32_t n);
  std::string recvline();
  void Send(const std::string&);
};
