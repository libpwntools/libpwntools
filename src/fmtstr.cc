#include <pwntools>
#include <libpwntools/fmtstr.h>
#include <libpwntools/utils.h>
#include <libpwntools/logger.h>
#include <iostream>
#include <algorithm>

pwn::fmtstr_payload::fmtstr_payload() {
    this->offset = 6;
    this->padding = 0;
    this->set_bytes_written(0);
}

pwn::fmtstr_payload::fmtstr_payload(uint32_t offset) {
    this->offset = offset;
    this->padding = 0;
    this->set_bytes_written(0);
}

pwn::fmtstr_payload::fmtstr_payload(uint32_t offset, uint32_t written) {
    this->offset = offset;
    this->padding = 0;
    this->set_bytes_written(written);
}

void pwn::fmtstr_payload::do_single_write(uint64_t addr, uint8_t value) {
    this->list.push_back({addr, value});
}

void pwn::fmtstr_payload::do_write(uint64_t addr, uint64_t value) {
    for(int i=0; i < 8; ++i) {
        this->do_single_write(addr + i, value & 0xff);
        value >>= 8;
    }
}

void pwn::fmtstr_payload::set_bytes_written(size_t n) {
    this->bytes_written = n;
    if(n)
        this->padding = 8 - (n % 8);
}

uint64_t &pwn::fmtstr_payload::operator[](uint64_t addr) {
    return this->writes[addr];
}

uint64_t pwn::fmtstr_payload::get_write_size() {
    size_t payload_size = 0;
    uint64_t written = this->bytes_written + this->padding;
    for(auto iter : this->list) {
        uint64_t value = iter.second;
        if(value == (written & 0xff)) {
            payload_size += 7;
            continue;
        }
        uint64_t write_size = (value - written) & 0xff;
        if(write_size < 10)
            payload_size += 10;
        else if(write_size < 100)
            payload_size += 11;
        else
            payload_size += 12;
        written += write_size;
    }
    return payload_size;
}

int64_t pwn::fmtstr_payload::find_offset(std::function<std::string(const std::string&)> func) {
    std::string egg = pwn::random_string(8);
    std::string egg_hex = pwn::Hex(pwn::u64(egg));
    std::string res;

    for(int i=1; i<0x1000; ++i) {
        res = func(egg+"%"+std::to_string(i)+"$p");
        if(res.find(egg) == std::string::npos)
            break;
        if(res.find(egg_hex) != std::string::npos) {
            this->offset = i;
            return i;
        }
    }

    pwn::log::failure("Could not find format string offset");
    return -1;
}

std::string pwn::fmtstr_payload::build() {
    for(auto x : this->writes)
        this->do_write(x.first, x.second);
 
    std::sort(this->list.begin(), this->list.end(),
        [](const std::pair<int,int> &a, const std::pair<int,int> &b) -> bool
        {
            return a.second < b.second;
        });

    std::string payload;
    uint64_t written = this->bytes_written + this->padding;

    uint64_t payload_size = this->get_write_size();
    payload_size = payload_size + 8 - (payload_size % 8);
    uint64_t pos = this->offset + (written + payload_size)/8;
    
    for(auto iter : this->list) {
        uint64_t value = iter.second;
        if(value != (written & 0xff)) {
            uint64_t len = (value - written) & 0xff;
            payload += "%" + std::to_string(len) + "c";
            written += len;
        }
        payload += "%" + std::to_string(pos++) + "$hhn";
    }

    payload += std::string(payload_size-payload.length(), '|');
    for(auto iter : this->list)
        payload += pwn::p64(iter.first);

    this->list.clear();
    this->writes.clear();
    return payload;
}

pwn::fmtstr_payload::~fmtstr_payload() {
    this->list.clear();
    this->writes.clear();
}
