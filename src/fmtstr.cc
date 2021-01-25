#include <libpwntools/fmtstr.h>

fmtstr_payload::fmtstr_payload(uint32_t offset) {
    this->offset = offset;
    this->set_bytes_written(0);
}

void fmtstr_payload::do_write(uint64_t addr, uint64_t value) {
    this->list.push_back({addr, value});
}

void fmtstr_payload::set_bytes_written(size_t n) {
    this->bytes_written = n;
}

std::string fmtstr_payload::gen_payload() {
    std::string payload;
    return payload;
}
