#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include <string>
#include <iostream>

HANDLE g_hChildStd_IN_Rd = NULL;
HANDLE g_hChildStd_IN_Wr = NULL;
HANDLE g_hChildStd_OUT_Rd = NULL;
HANDLE g_hChildStd_OUT_Wr = NULL;

void CreateChildProcess() {

	TCHAR szCmdline[] = TEXT("./cat.exe");
	PROCESS_INFORMATION piProcInfo;
	STARTUPINFO siStartInfo;
	BOOL bSuccess = FALSE;

	ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));

	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.hStdError = g_hChildStd_OUT_Wr;
	siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
	siStartInfo.hStdInput = g_hChildStd_IN_Rd;
	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

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
		std::cout << "Error ( Creating Process ) " << std::endl;
		exit(-1);
	}
	else {

		std::cout << "CREATED" << std::endl;
		CloseHandle(piProcInfo.hProcess);
		CloseHandle(piProcInfo.hThread);

		CloseHandle(g_hChildStd_OUT_Wr);
		CloseHandle(g_hChildStd_IN_Rd);
	}
}

void ReadFromPipe(void)
{
	CHAR buf[] = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
	DWORD dwRead, dwWritten;
	CHAR chBuf[0x100];
	BOOL bSuccess = FALSE;
	HANDLE hParentStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	std::cout << "NOW" << std::endl;
	bSuccess = ReadFile(g_hChildStd_OUT_Rd, chBuf, sizeof(buf), &dwRead, NULL);
	if (!bSuccess || dwRead == 0) {
		std::cout << "Error reading" << std::endl;
		exit(-1);
	}

	std::cout << "Reading" << std::endl;
	bSuccess = WriteFile(hParentStdOut, chBuf,
		dwRead, &dwWritten, NULL);
	if (!bSuccess) {
		std::cout << "Error Reading" << std::endl;
		exit(-1);
	}
}

void WriteToPipe(void)
{
	DWORD dwWritten;
	char s[0x100];
	std::string b;

	std::cout << "Paused" << std::endl;
	std::cin >> b;
	b += getchar();

	BOOL bSuccess = FALSE;

	bSuccess = WriteFile(g_hChildStd_IN_Wr, b.c_str(), b.size(), &dwWritten, NULL);
	if (!bSuccess) {

		std::cout << "Error writing" << std::endl;
		exit(-1);
	}
}

int main() {

	SECURITY_ATTRIBUTES saAttr { NULL };

	char buffer[] = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = true;
	saAttr.lpSecurityDescriptor = NULL;

	if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0)) {
		std::cout << "Error " << std::endl;
		exit(-1);
	}

	if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0)) {
		std::cout << "Error " << std::endl;
		exit(-1);
	}

	if (!CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0)) {
		std::cout << "Error" << std::endl;
		exit(-1);
	}

	if (!SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0)) {
		std::cout << "Error" << std::endl;
		exit(-1);

	}
	CreateChildProcess();
	WriteToPipe();
	ReadFromPipe();

	return 0;
}

