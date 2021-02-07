#pragma once
#include <libpwntools/Socket.h>
#include <string>
#include <libpwntools/remote.h>
#include <libpwntools/io.h>
#include <libpwntools/utils.h>

class Remote : public IO {
 private:
  std::string host;
  std::string port;
  Socket* sock;
  std::vector<Socket> reads;
 public:
  Remote();
  Remote(const std::string&, uint32_t);
  std::string recv(size_t);
  size_t send(const std::string&);
  void shutdown(const std::string& );
	void close();
};