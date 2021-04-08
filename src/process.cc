/*
BSD 2-Clause License

Copyright (c) 2021, libpwntools
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <iostream>
#ifdef __linux__
#include <bits/stdc++.h>
#include <libpwntools/process.h>
#include <libpwntools/utils.h>
#include <signal.h>
#include <stdint.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#elif _WIN32
#include "logger.h"
#include "process.h"
#include "utils.h"
#include <Windows.h>
#endif

#include <signal.h>

#include <thread>

pwn::Process::Process() {
}

pwn::Process::~Process() {
    if (this->status == running)
        return;
    this->close();
}

pwn::Process::Process(const std::string &path) {
#ifdef __linux__
    int inpipefd[2];
    int outpipefd[2];

    pipe(inpipefd);
    pipe(outpipefd);

    this->pid = fork();
    if (!this->pid) {
        dup2(outpipefd[0], 0);
        dup2(inpipefd[1], 1);
        dup2(inpipefd[1], 2);

        std::vector<char *> av;
        prctl(PR_SET_PDEATHSIG, SIGTERM);
        av.push_back((char *)path.c_str());
        av.push_back(nullptr);

        execv(path.c_str(), &av[0]);
        exit(0);
    }

    ::close(inpipefd[1]);
    ::close(outpipefd[0]);
    this->_stdin = outpipefd[1];
    this->_stdout = inpipefd[0];
#elif _WIN32
    this->saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    this->saAttr.bInheritHandle = true;
    this->saAttr.lpSecurityDescriptor = nullptr;

    if (!CreatePipe(&this->g_hChildStd_OUT_Rd, &this->g_hChildStd_OUT_Wr,
                    &this->saAttr, 0))
        pwn::abort("Error Creating Pipe");

    if (!SetHandleInformation(this->g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
        pwn::abort("Error SetHandle");

    if (!CreatePipe(&this->g_hChildStd_IN_Rd, &this->g_hChildStd_IN_Wr, &saAttr,
                    0))
        pwn::abort("Error Createing Pipe");

    if (!SetHandleInformation(this->g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0))
        pwn::abort("Error SetHandle");

    createProcess((const char *)path.c_str());
#endif
    this->debug = false;
    this->status = running;
}

void pwn::Process::debugger_attach() {
    std::string _pid = std::to_string(this->pid);
    std::string cmd;
#ifdef __linux__
    cmd = "gnome-terminal -- gdb --pid=" + _pid;
#elif _WIN32
    cmd = "start WinDbgx.exe -p " + _pid;
#else
    exit(1);
#endif
    system(cmd.c_str());
}

std::string pwn::Process::recv_raw(size_t len) {
    char *buf = (char *)malloc(len);
#ifdef __linux__
    len = read(this->_stdout, buf, len);
    std::string s(buf, len);
#elif _WIN32
    DWORD dwRead;
    BOOL bSuccess = FALSE;
    bSuccess = ReadFile(this->g_hChildStd_OUT_Rd, buf, len, &dwRead, nullptr);
    if (!bSuccess)
        pwn::abort("Error reading");
    std::string s(buf, dwRead);
#else
    exit(0);
#endif
    free(buf);
    return s;
}

size_t pwn::Process::send(const std::string &buf) {
    if (this->debug) {
        std::cout << "Send: \n";
        pwn::hexdump(buf);
    }
#ifdef __linux__
    return write(this->_stdin, buf.c_str(), buf.length());
#elif _WIN32
    DWORD dwWritten;
    BOOL bSuccess;

    return WriteFile(this->g_hChildStd_IN_Wr, buf.c_str(), buf.size(),
                     &dwWritten, nullptr);
#endif
}

void pwn::Process::close() {
#ifdef __linux__
    kill(this->pid, SIGKILL);
    waitpid(this->pid, nullptr, 0);
    ::close(this->_stdin);
    ::close(this->_stdout);
#elif _WIN32
    exit(1);
#endif
    this->status = closed;
}

#ifdef _WIN32
void pwn::Process::createProcess(const char *progname) {
    BOOL bSuccess = FALSE;

    ZeroMemory(&this->piProcInfo, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&this->siStartInfo, sizeof(STARTUPINFO));

    siStartInfo.cb = sizeof(STARTUPINFO);
    siStartInfo.hStdError = this->g_hChildStd_OUT_Wr;
    siStartInfo.hStdOutput = this->g_hChildStd_OUT_Wr;
    siStartInfo.hStdInput = this->g_hChildStd_IN_Rd;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    LPSTR szCmdline = const_cast<LPSTR>(progname);
    bSuccess = CreateProcess(nullptr, szCmdline, nullptr, nullptr, TRUE, 0,
                             nullptr, nullptr, &siStartInfo, &piProcInfo);

    if (!bSuccess)
        pwn::abort("Error creating process");
    else {
        CloseHandle(piProcInfo.hProcess);
        CloseHandle(piProcInfo.hThread);

        CloseHandle(g_hChildStd_OUT_Wr);
        CloseHandle(g_hChildStd_IN_Rd);
        this->pid = piProcInfo.dwProcessId;
        pwn::log::info("Process created: " + std::to_string(this->pid));
    }
}
#endif
