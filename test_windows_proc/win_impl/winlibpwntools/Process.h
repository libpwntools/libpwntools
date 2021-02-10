#pragma once	
#include <windows.h>
#include <stdio.h>
#include <strsafe.h>
#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include "process.h"
#include <tchar.h>

class Process {

	public:
		Process(const char*);
		size_t send(const std::string&);
		size_t sendline(const std::string&);
		std::string recv(size_t);
		std::string recvuntil(const std::string&);
		void recvloop();
		std::string recvline();
		size_t sendafter(const std::string&, const std::string&);
		size_t sendlineafter(const std::string&, const std::string&);
		void interactive();
	private:
		void createProcess(const char*);
		HANDLE g_hChildStd_IN_Rd;
		HANDLE g_hChildStd_IN_Wr;
		HANDLE g_hChildStd_OUT_Rd;
		HANDLE g_hChildStd_OUT_Wr;
		SECURITY_ATTRIBUTES saAttr;
		PROCESS_INFORMATION piProcInfo;
		STARTUPINFO siStartInfo;
};

bool ends_with(const std::string&, const std::string&);