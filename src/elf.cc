#include <libpwntools/elf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/stat.h>

ELF::ELF(const std::string &filename) {
    int fd = open(filename.c_str(), O_RDONLY);
    assert(fd > 0);

    struct stat st;
    stat(filename.c_str(), &st);

    unsigned char * tmp = (unsigned char *)malloc(st.st_size);
    read(fd, tmp, st.st_size);
    this->file = std::string((const char *)tmp, st.st_size);
    free(tmp);

    close(fd);
}

int64_t ELF::find_str_offset(const std::string &str) {
    int64_t idx = this->file.find(str);
    if(idx == std::string::npos) return -1;
    return idx;
}
