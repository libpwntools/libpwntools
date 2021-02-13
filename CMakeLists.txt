# CMakeList.txt : CMake project for libpwntools, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)
project(libpwntools)
if(UNIX)
        set(CMAKE_EXE_LINKER_FLAGS "-pthread")

        file(GLOB SOURCES src/*.cc)
        include_directories(include src/)

        add_library(pwntools SHARED ${SOURCES})
        install(TARGETS pwntools DESTINATION /lib/)

        file(GLOB HEADERS include/*.h src/*.h)
        install(FILES ${HEADERS} DESTINATION include/libpwntools/)

        file(GLOB HEADERS include/*.h src/pwntools)
        install(FILES ${HEADERS} DESTINATION include/)
else()
	add_executable (exp "src/main.cc" "src/fmtstr.cc" "src/fmtstr.h" "src/pwntools.cc" "src/pwntools" "src/logger.cc" "src/logger.h" "src/utils.cc" "src/utils.h"  "src/io.cc" "src/io.h" "src/process.cc" "src/process.h" "src/remote.cc" "src/remote.h")

endif()
