#include "Process.h"
#include <Windows.h>

bool ends_with(const std::string& a, const std::string& b) {
	if (b.size() > a.size()) return false;
	return std::equal(a.begin() + a.size() - b.size(), a.end(), b.begin() );
}

Process::Process(const char *progname) {
	this->saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	this->saAttr.bInheritHandle = true;
	this->saAttr.lpSecurityDescriptor = NULL;

	if (!CreatePipe(&this->g_hChildStd_OUT_Rd, &this->g_hChildStd_OUT_Wr, &this->saAttr, 0)) {
		std::cout << "[+] Error Creating Pipe" << std::endl;
		exit(-1);
	}

	if (!SetHandleInformation(this->g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0)) {
		std::cout << "[+] Error		" << std::endl;
		exit(-1);
	}

	if (!CreatePipe(&this->g_hChildStd_IN_Rd, &this->g_hChildStd_IN_Wr, &saAttr, 0)) {
		std::cout << "Error" << std::endl;
		exit(-1);
	}

	if (!SetHandleInformation(this->g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0)) {
		std::cout << "Error" << std::endl;
		exit(-1);
	}

	createProcess(progname);
}

size_t Process::send(const std::string& s) {
	DWORD dwWritten;
	BOOL bSuccess;

	bSuccess = WriteFile(this->g_hChildStd_IN_Wr, s.c_str(), s.size(), &dwWritten, NULL);
	if (!bSuccess) {
		std::cout << "[+] Error Writing" << std::endl;
		exit(-1);
	}
	return dwWritten;
}

std::string Process::recv(size_t len) {
	char* buff = new char[len + 1];
	DWORD dwRead;
	BOOL bSuccess = FALSE;
	bSuccess = ReadFile(this->g_hChildStd_OUT_Rd, buff, len, &dwRead, NULL);
	if (!bSuccess) {
		std::cout << "[+] Error reading" << std::endl;
		exit(-1);
	}
	std::string return_buf(buff, dwRead);
	delete[] buff;
	return return_buf;
}

void Process::recvloop() {
	while (true) {
		std::cout << this->recv(1024);
	}
}

size_t Process::sendline(const std::string& buf) {
	return this->send(buf + "\n");
}
std::string Process::recvuntil(const std::string& buf) {

	std::string s;
	while (!ends_with(s, buf))
		s += this->recv(1);
	return s;
}

std::string Process::recvline() {
	return this->recvuntil("\n");
}
	
size_t Process::sendafter(const std::string& recv_data, const std::string& send_data) {
	this->recvuntil(recv_data);
	return this->send(send_data);
}

size_t Process::sendlineafter(const std::string& recv_data, const std::string& send_data) {
	this->recvuntil(recv_data);
	return this->sendline(send_data);
}

void Process::interactive() {

	std::cout.setf(std::ios::unitbuf);
	std::cin.setf(std::ios::unitbuf);
	std::thread t1(&Process::recvloop, this);
	SleepEx(150,false);
	std::string input;
	while (true) {
		std::cout << "$ ";
		getline(std::cin, input);
		this->sendline(input);
		SleepEx(150,false);
	}
}

void Process::createProcess(const char* progname) {

	BOOL bSuccess = FALSE;

	ZeroMemory(&this->piProcInfo, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&this->siStartInfo, sizeof(STARTUPINFO));

	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.hStdError = this->g_hChildStd_OUT_Wr;
	siStartInfo.hStdOutput = this->g_hChildStd_OUT_Wr;
	siStartInfo.hStdInput = this->g_hChildStd_IN_Rd;
	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

	wchar_t* szCmdline = new wchar_t[strlen(progname) + 1];
	size_t converted_chars;
	mbstowcs_s(&converted_chars, szCmdline, strlen(progname) + 1,(char *)progname, _TRUNCATE);
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
		std::cout << "[+] Process created." << std::endl;
	}

}