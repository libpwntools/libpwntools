#include <iostream>
#include <bits/stdc++.h>
#include <stdint.h>
#include "process.h"
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/prctl.h>

Process::Process() {}
Process::Process(const std::string &path) {
    int inpipefd[2];
    int outpipefd[2];

    pipe(inpipefd);
    pipe(outpipefd);

    this->pid = fork();
    if(!this->pid) {
        dup2(outpipefd[0], 0);
        dup2(inpipefd[1], 1);
        dup2(inpipefd[1], 2);

        std::vector<char*> av;
        prctl(PR_SET_PDEATHSIG, SIGTERM);
        av.push_back((char *)path.c_str());
        av.push_back(NULL);

        execv(path.c_str(), &av[0]);
        exit(0);
    }

    close(inpipefd[1]);
    close(outpipefd[0]);
    this->_stdin = outpipefd[1];
    this->_stdout = inpipefd[0];
}

std::string Process::recv(size_t len) {
    char * buf = (char *)malloc(len);
    len = read(this->_stdout, buf, len);
    std::string s(buf, len);
    free(buf);
    return s;
}

size_t Process::send(const std::string &buf) {
    return write(this->_stdin, buf.c_str(), buf.length());
}

/* Example:

  int main(void) {
      Process io("/bin/cat");
      io.send("Pepega");
      std::cout << io.recv(1024);
  }

*/
