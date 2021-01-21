#include <iostream>
#include <bits/stdc++.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/prctl.h>
#include <thread>
#include "process.h"
#include "utils.h"

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

void Process::gdb_attach() {
    std::string _pid = std::to_string(this->pid);
    std::string cmd = "gnome-terminal -- gdb --pid="+_pid;
    system(cmd.c_str());
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

void Process::recvloop() {
    std::string s;
    while(true) {
        s = this->recv(1024);
        write(1, s.c_str(), s.length());
        s.clear();
    }
}

size_t Process::sendline(const std::string &buf) {
    return this->send(buf + "\n");
}

std::string Process::recvuntil(const std::string &buf) {
    std::string s;
    while (!ends_with(s, buf))
        s += this->recv(1);
    return s;
}

std::string Process::recvline() {
    return this->recvuntil("\n");
}

void Process::interactive() {
    std::cout.setf(std::ios::unitbuf);
    std::cin.setf(std::ios::unitbuf);

    std::thread t1(&Process::recvloop, this);
    usleep(1500);

    std::string inp;
    while(true) {
        std::cout << "$ ";
        getline(std::cin, inp);
        this->sendline(inp);
        usleep(10000);
    }
}
