#include <libpwntools/logger.h>
#include <libpwntools/utils.h>

#include <iostream>
#include <string>
#ifdef __linux__
#include <bits/stdc++.h>

#include <ostream>
#elif _WIN32
#include <sstream>
#endif
#include <random>

bool pwn::ends_with(const std::string &a, const std::string &b) {
    if (b.size() > a.size()) return false;
    return std::equal(a.begin() + a.size() - b.size(), a.end(), b.begin());
}

void pwn::hexdump_wrap(void *pAddressIn, size_t lSize) {
    char szBuf[100];
    long lIndent = 1;
    long lOutLen, lIndex, lIndex2, lOutLen2;
    long lRelPos;
    struct {
        char *pData;
        unsigned long lSize;
    } buf;
    unsigned char *pTmp, ucTmp;
    unsigned char *pAddress = (unsigned char *)pAddressIn;

    buf.pData = (char *)pAddress;
    buf.lSize = lSize;

    while (buf.lSize > 0) {
        pTmp = (unsigned char *)buf.pData;
        lOutLen = (int)buf.lSize;

        if (lOutLen > 16) lOutLen = 16;

        sprintf(szBuf,
                " >                            "
                "                      "
                "    %08lX",
                pTmp - pAddress);
        lOutLen2 = lOutLen;

        for (lIndex = 1 + lIndent, lIndex2 = 53 - 15 + lIndent, lRelPos = 0;
             lOutLen2; lOutLen2--, lIndex += 2, lIndex2++) {
            ucTmp = *pTmp++;
            sprintf(szBuf + lIndex, "%02X ", (unsigned short)ucTmp);

            if (!isprint(ucTmp)) ucTmp = '.';

            szBuf[lIndex2] = ucTmp;
            if (!(++lRelPos & 3)) {
                lIndex++;
                szBuf[lIndex + 2] = ' ';
            }
        }

        if (!(lRelPos & 3)) lIndex--;

        szBuf[lIndex] = '<';
        szBuf[lIndex + 1] = ' ';

        printf("%s\n", szBuf);
        buf.pData += lOutLen;
        buf.lSize -= lOutLen;
    }
}

void pwn::hexdump(std::string s) {
    pwn::hexdump_wrap((void *)s.c_str(), s.size());
}

std::string pwn::str_repeat(const std::string &str, size_t n) {
    std::string s;
    for (int i = 0; i < n; ++i) s += str;
    return s;
}

std::string pwn::str_xor(const std::string &a, const std::string &b) {
    std::string c;
    size_t len_a = a.length();
    size_t len_b = b.length();
    assert(len_a && len_b);

    if (len_a > len_b) {
        c = a;
        for (int i = 0; i < len_a; ++i) c[i] ^= b[i % len_b];
    } else {
        c = b;
        for (int i = 0; i < len_b; ++i) c[i] ^= a[i % len_a];
    }
    return c;
}

std::string pwn::string_to_hex(const std::string &input) {
    static const char hex_digits[] = "0123456789ABCDEF";

    std::string output;
    output.reserve(input.length() * 2);
    for (unsigned char c : input) {
        output.push_back(hex_digits[c >> 4]);
        output.push_back(hex_digits[c & 15]);
    }
    return output;
}

int pwn::hex_value(unsigned char hex_digit) {
    static const signed char hex_values[256] = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0,  1,  2,  3,  4,  5,
        6,  7,  8,  9,  -1, -1, -1, -1, -1, -1, -1, 10, 11, 12, 13, 14, 15, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1,
    };
    int value = hex_values[hex_digit];
    if (value == -1) throw std::invalid_argument("invalid hex digit");
    return value;
}

std::string pwn::hex_to_string(const std::string &input) {
    const auto len = input.length();
    if (len & 1) throw std::invalid_argument("odd length");

    std::string output;
    output.reserve(len / 2);
    for (auto it = input.begin(); it != input.end();) {
        int hi = pwn::hex_value(*it++);
        int lo = pwn::hex_value(*it++);
        output.push_back(hi << 4 | lo);
    }
    return output;
}

std::string pwn::remove_newline(std::string &s) {
    int pos;
    if ((pos = s.find('\n')) != std::string::npos) s.erase(pos);
    return s;
}

std::string pwn::Hex(uint64_t n) {
    std::stringstream ss;
    ss << std::hex << n;
    std::string res = "0x" + ss.str();
    return res;
}

uint64_t pwn::Int(const std::string &n, uint8_t base) {
    if (base < 2 || base > 36) pwn::abort("Invalid base");

    char *p;
    uint64_t val = strtoul(n.c_str(), &p, base);
    if (*p) pwn::abort("Not a number");

    return val;
}

std::string pwn::random_string(size_t len) {
    constexpr std::string_view charset =
        "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string s;
    constexpr size_t charset_length = charset.length();
    srand(time(nullptr));

    for (int i = 0; i < len; ++i) s += charset[rand() % charset_length];
    return s;
}

void pwn::abort(const std::string &s) {
    pwn::log::error(s);
    exit(1);
}
