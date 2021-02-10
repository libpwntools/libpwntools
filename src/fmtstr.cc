#include <libpwntools/fmtstr.h>
#include <libpwntools/utils.h>
#include <iostream>

bool sortcmp(const std::pair<int,int> &a, const std::pair<int,int> &b) {
    return (a.second < b.second);
}

fmtstr_payload::fmtstr_payload() {
    this->offset = 6;
    this->padding = 0;
    this->set_bytes_written(0);
}

fmtstr_payload::fmtstr_payload(uint32_t offset) {
    this->offset = offset;
    this->padding = 0;
    this->set_bytes_written(0);
}

fmtstr_payload::fmtstr_payload(uint32_t offset, uint32_t written) {
    this->offset = offset;
    this->padding = 0;
    this->set_bytes_written(written);
}

void fmtstr_payload::do_single_write(uint64_t addr, uint8_t value) {
    this->list.push_back({addr, value});
}

void fmtstr_payload::do_write(uint64_t addr, uint64_t value) {
    for(int i=0; i < 8; ++i) {
        this->do_single_write(addr + i, value & 0xff);
        value >>= 8;
    }
}

void fmtstr_payload::set_bytes_written(size_t n) {
    this->bytes_written = n;
    if(n)
        this->padding = 8 - (n % 8);
}

uint64_t &fmtstr_payload::operator[](uint64_t addr) {
    return this->writes[addr];
}

uint64_t fmtstr_payload::get_write_size() {
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

std::string fmtstr_payload::build() {
    for(auto x : this->writes)
        this->do_write(x.first, x.second);
 
    std::sort(this->list.begin(), this->list.end(), sortcmp);
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
        payload += std::string((char *)&iter.first, 8);

    this->list.clear();
    this->writes.clear();
    return payload;
}

fmtstr_payload::~fmtstr_payload() {
    this->list.clear();
    this->writes.clear();
}
