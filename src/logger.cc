#include <libpwntools/logger.h>

void logger::success(const std::string& msg) {
    std::cout << "[\x1b[1m\x1b[32m+\x1b[m] " << msg << '\n';
}

void logger::info(const std::string& msg) {
    std::cout << "[\x1b[1m\x1b[34m*\x1b[m] " << msg << '\n';
}

void logger::failure(const std::string& msg) {
    std::cout << "[\33[1m\33[31m-\33[m] " << msg << '\n';
}

void logger::error(const std::string& msg) {
    std::cout << "[\x1b[41mERROR\x1b[m] " << msg << '\n';
}

void logger::warning(const std::string& msg) {
    std::cout << "[\x1b[1m\x1b[33m!\x1b[m] " << msg << '\n';
}
