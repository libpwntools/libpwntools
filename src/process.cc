#include <iostream>
#ifdef __linux__
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/prctl.h>
#include <bits/stdc++.h>
#include <stdint.h>
#elif _WIN32
#include <Windows.h>
#endif
#include <thread>
#include <signal.h>
#include <libpwntools/process.h>
#include <libpwntools/utils.h>

pwn::Process::Process() {}

pwn::Process::~Process() {
    this->close();
}

pwn::Process::Process(const std::string& path) {
#ifdef __linux__
    int inpipefd[2];
    int outpipefd[2];
    if (pipe(inpipefd) < 0) {
        std::cout << "[+] Error Pipe" << std::endl;
        exit(1);
    }
    if (pipe(outpipefd) < 0) {
        std::cout << "[+] Error Pipe" << std::endl;
        exit(1);
    }
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
    
#elif _WIN32
    this->saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    this->saAttr.bInheritHandle = true;
    this->saAttr.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&this->g_hChildStd_OUT_Rd, &this->g_hChildStd_OUT_Wr, &this->saAttr, 0)) {
        std::cout << "[+] Error Creating Pipe" << std::endl;
        exit(-1);
    }

    if (!SetHandleInformation(this->g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0)) {
        std::cout << "[+] Error	SetHandle" << std::endl;
        exit(-1);
    }

    if (!CreatePipe(&this->g_hChildStd_IN_Rd, &this->g_hChildStd_IN_Wr, &saAttr, 0)) {
        std::cout << "[+] Error Createing Pipe" << std::endl;
        exit(-1);
    }

    if (!SetHandleInformation(this->g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0)) {
        std::cout << "[+] Error SetHandle" << std::endl;
        exit(-1);
    }

    createProcess((const char *)path.c_str());
#endif
    this->debug = false;
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
    char* buff = new char[len + 1];
#ifdef __linux__
    len = read(this->_stdout, buff, len);
    std::string return_buf(buff, len);
    delete[] buff;
#elif _WIN32
    DWORD dwRead;
    BOOL bSuccess = FALSE;
    bSuccess = ReadFile(this->g_hChildStd_OUT_Rd, buff, len, &dwRead, NULL);
    if (!bSuccess) {
        std::cout << "[+] Error reading" << std::endl;
        exit(-1);
    }
    std::string return_buf(buff, dwRead);
    delete[] buff;    
#else
    exit(0);
#endif
    return return_buf;
}

size_t pwn::Process::send(const std::string &buf) {
    if(this->debug) {
        std::cout << "Send: \n";
        pwn::hexdump(buf);
    }
#ifdef __linux__
    return write(this->_stdin, buf.c_str(), buf.length());
#elif _WIN32
    DWORD dwWritten;
    BOOL bSuccess;

    bSuccess = WriteFile(this->g_hChildStd_IN_Wr, buf.c_str(), buf.size(), &dwWritten, NULL);
    if (!bSuccess) {
        std::cout << "[+] Error Writing" << std::endl;
        exit(-1);
    }
    return (size_t) dwWritten;
#endif
}

void pwn::Process::close() {
#ifdef __linux__
    kill(this->pid,SIGKILL);
    ::close(this->_stdin);
    ::close(this->_stdout);
#elif _WIN32
    exit(1);
#endif
}

#ifdef _WIN32
void pwn::Process::createProcess(const char* progname) {

    BOOL bSuccess = FALSE;

    ZeroMemory(&this->piProcInfo, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&this->siStartInfo, sizeof(STARTUPINFO));

    siStartInfo.cb = sizeof(STARTUPINFO);
    siStartInfo.hStdError = this->g_hChildStd_OUT_Wr;
    siStartInfo.hStdOutput = this->g_hChildStd_OUT_Wr;
    siStartInfo.hStdInput = this->g_hChildStd_IN_Rd;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    LPSTR szCmdline = const_cast<LPSTR>(progname);
    bSuccess = CreateProcess(NULL,
        szCmdline,
        NULL,
        NULL,
        TRUE,
        0,
        NULL,
        NULL,
        &siStartInfo,
        &piProcInfo);

    if (!bSuccess) {
        std::cout << "[+] Error creating process" << std::endl;
        exit(-1);
    }
    else {
        CloseHandle(piProcInfo.hProcess);
        CloseHandle(piProcInfo.hThread);

        CloseHandle(g_hChildStd_OUT_Wr);
        CloseHandle(g_hChildStd_IN_Rd);
        this->pid = piProcInfo.dwProcessId;
        std::cout << "[+] Process created: " << this->pid << std::endl;
    }
}
#endif
