# CMakeList.txt : CMake project for libpwntools, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)
if(UNIX)
	set(CMAKE_EXE_LINKER_FLAGS "-pthread")
endif()
# Add source to this project's executable. #oops
add_executable (ayy "main.cc" "pwntools.cc" "pwntools.h" "logger.cc" "logger.h" "utils.cc" "utils.h" "fmtstr.cc" "fmtstr.h" "io.cc" "io.h" "process.cc" "process.h" "remote.cc" "remote.h")
# TODO: Add tests and install targets if needed.
