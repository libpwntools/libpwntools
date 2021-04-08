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
