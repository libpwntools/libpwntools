#ifdef __linux__
#include <libpwntools/logger.h>
#elif _WIN32
#include "logger.h"
#endif

void pwn::log::success(const std::string &msg) {
    std::cout << "[\x1b[1m\x1b[32m+\x1b[m] " << msg << '\n';
}

void pwn::log::info(const std::string &msg) {
    std::cout << "[\x1b[1m\x1b[34m*\x1b[m] " << msg << '\n';
}

void pwn::log::failure(const std::string &msg) {
    std::cout << "[\33[1m\33[31m-\33[m] " << msg << '\n';
}

void pwn::log::error(const std::string &msg) {
    std::cout << "[\x1b[41mERROR\x1b[m] " << msg << '\n';
}

void pwn::log::warning(const std::string &msg) {
    std::cout << "[\x1b[1m\x1b[33m!\x1b[m] " << msg << '\n';
}
