#pragma once
#ifdef __linux__
#include <libpwntools/io.h>
#elif _WIN32
#include "io.h"
#include <stdio.h>
#include <strsafe.h>
#include <tchar.h>
#include <windows.h>
#endif

#ifndef PROCESS_H
#define PROCESS_H
namespace pwn {
    class Process : public pwn::IO {
      public:
        Process();
        ~Process();
        Process(const std::string &);
        virtual size_t send(const std::string &) override;
        virtual std::string recv_raw(size_t len) override;
        void debugger_attach();
        virtual void close() override;

      private:
#ifdef __linux__
        int _stdin;
        int _stdout;
#elif _WIN32
        void createProcess(const char *);
        HANDLE g_hChildStd_IN_Rd;
        HANDLE g_hChildStd_IN_Wr;
        HANDLE g_hChildStd_OUT_Rd;
        HANDLE g_hChildStd_OUT_Wr;
        SECURITY_ATTRIBUTES saAttr;
        PROCESS_INFORMATION piProcInfo;
        STARTUPINFO siStartInfo;
#endif
        int pid;
    };
} // namespace pwn
#endif
