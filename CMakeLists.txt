# CMakeList.txt : CMake project for libpwntools, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)
if(UNIX)
        set(CMAKE_EXE_LINKER_FLAGS "-pthread")

        file(GLOB SOURCES src/*.cc)
        include_directories(include)

        add_library(pwntools SHARED ${SOURCES})

        install(TARGETS pwntools DESTINATION lib/pwntools)

        file(GLOB HEADERS include/*.h)
        install(FILES ${HEADERS} DESTINATION include/${PROJECT_NAME})
endif()
