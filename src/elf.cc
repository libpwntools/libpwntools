#include <iostream>
#include <libpwntools/elf.h>
#include <libpwntools/utils.h>
#include <elf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <cassert>

pwn::ELF::ELF(const std::string &filename) {
    int fd = open(filename.c_str(), O_RDONLY);
    assert(fd > 0);
    struct stat st;

    stat(filename.c_str(), &st);
    unsigned char * tmp = (unsigned char *)malloc(st.st_size);
    read(fd, tmp, st.st_size);
    this->file = std::string((const char *)tmp, st.st_size);
    free(tmp);

    Elf64_Ehdr *elf_header = (Elf64_Ehdr *)this->file.c_str();
    assert(elf_header->e_ident[EI_CLASS] == ELFCLASS64);
    close(fd);
}

std::vector<pwn::section> pwn::ELF::parse_sections() {
    uint8_t * raw = (uint8_t *)this->file.c_str();
    Elf64_Ehdr *elf_header = (Elf64_Ehdr *)raw;
    Elf64_Shdr *section_header = (Elf64_Shdr *)&raw[elf_header->e_shoff];

    size_t shnum = elf_header->e_shnum;
    Elf64_Shdr *sh_strtab = &section_header[elf_header->e_shstrndx];
    uint8_t * sh_strtab_p = &raw[sh_strtab->sh_offset];

    std::vector<pwn::section> vect;
    for(int i=0; i<shnum; ++i) {
        section s;
        s.section_name = std::string((char *)(&sh_strtab_p[section_header[i].sh_name]));
        s.section_type = section_header[i].sh_type;
        s.section_offset = section_header[i].sh_offset;
        s.section_address = section_header[i].sh_addr;
        s.section_size = section_header[i].sh_size;
        s.section_entsize = section_header[i].sh_entsize;
        s.section_addr_align = section_header[i].sh_addralign;
        vect.push_back(s);
    }
    return vect;
}

int64_t pwn::ELF::find_str_offset(const std::string &str) {
    int64_t idx = this->file.find(str);
    if(idx == std::string::npos) return -1;
    return idx;
}
