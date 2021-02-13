#pragma once
#include <iostream>
#include <string>

#ifndef LOGGER_H
#define LOGGER_H
namespace pwn {
    namespace log {
        void success(const std::string &msg);
        void info(const std::string &msg);
        void failure(const std::string &msg);
        void error(const std::string &msg);
        void warning(const std::string &msg);
    } // namespace log
} // namespace pwn
#endif
