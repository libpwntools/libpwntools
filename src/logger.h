#include <string>
#include <iostream>

namespace log {
    void success(const std::string& msg) {
        std::cout << "[\x1b[1m\x1b[32m+\x1b[m] " << msg << '\n';
    }

    void info(const std::string& msg) {
        std::cout << "[\x1b[1m\x1b[34m*\x1b[m] " << msg << '\n';
    }

    void failure(const std::string& msg) {
        std::cout << "[\33[1m\33[31m-\33[m] " << msg << '\n';
    }

    void error(const std::string& msg) {
        std::cout << "[\x1b[41mERROR\x1b[m] " << msg << '\n';
    }

    void warning(const std::string& msg) {
        std::cout << "[\x1b[1m\x1b[33m!\x1b[m] " << msg << '\n';
    }
}
