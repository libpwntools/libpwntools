#include <stdint.h>
#include <iostream>
#include <string>
#include <vector>
#include "Process.h"
#include <tchar.h>
#include <windows.h>

int main() {
	Process io("./cat.exe");

	io.interactive();
	return 0;
}