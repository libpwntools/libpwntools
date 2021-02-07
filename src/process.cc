#include <iostream>
#include <bits/stdc++.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/prctl.h>
#include <thread>
#include <signal.h>
#include <libpwntools/process.h>
#include <libpwntools/utils.h>

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

    ::close(inpipefd[1]);
    ::close(outpipefd[0]);
    this->_stdin = outpipefd[1];
    this->_stdout = inpipefd[0];
    this->debug = false;
}

void Process::gdb_attach() {
    std::string _pid = std::to_string(this->pid);
    std::string cmd = "gnome-terminal -- gdb --pid="+_pid;
    system(cmd.c_str());
}

std::string Process::recv(size_t len) {
    char* buf = new char[len + 1];
    len = read(this->_stdout, buf, len);
    std::string s(buf, len);
    delete buf;
    if(this->debug && len > 1) {std::cout << "Recv: \n"; hexdump(s);}
    return s;
}

size_t Process::send(const std::string &buf) {
    if(this->debug) {std::cout << "Send: \n"; hexdump(buf);}
    return write(this->_stdin, buf.c_str(), buf.length());
}

void Process::close() {
    kill(this->pid,SIGKILL);
    ::close(this->_stdin);
    ::close(this->_stdout);
}
