#pragma once
#include <libpwntools/Socket.h>
#include <string>
#include <libpwntools/remote.h>

class Remote {
 private:
  std::string host;
  std::string port;
  Socket* sock;
  std::vector<Socket> reads;
  bool debug;
 public:
  Remote();
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
  void shutdown(const std::string& );
	void close();
  void set_debug(bool);
};

void hexdump(void *, long);
