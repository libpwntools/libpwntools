#include <iostream>
#include <libpwntools/pwnelf.h>
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
    close(fd);

    this->file = std::string((const char *)tmp, st.st_size);
    free(tmp);
    Elf64_Ehdr *elf_header = (Elf64_Ehdr *)this->file.c_str();
    assert(elf_header->e_ident[EI_CLASS] == ELFCLASS64);

    std::vector<pwn::symbol> syms = this->parse_symbols();
    for(pwn::symbol sym : syms) {
        std::string sym_name = sym.symbol_name;
        uint64_t sym_value = sym.symbol_value;
        if(sym_name.length() && sym_value)
            this->sym_map[sym_name] = sym_value;
    }
}

uint64_t &pwn::ELF::operator[](const std::string &sym_name) {
    return this->sym_map[sym_name];
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
        pwn::section s;
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

std::vector<pwn::symbol> pwn::ELF::parse_symbols() {
    uint8_t * raw = (uint8_t *)this->file.c_str();
    std::vector<pwn::section> sections = this->parse_sections();
    std::vector<pwn::symbol> vect;

    uint8_t *sh_strtab_p = nullptr;
    for(pwn::section sec: sections) {
        if((sec.section_type == SHT_STRTAB) && (sec.section_name == ".strtab")){
            sh_strtab_p = &raw[sec.section_offset];
            break;
        }
    }

    uint8_t *sh_dynstr_p = nullptr;
    for(pwn::section sec: sections) {
        if((sec.section_type == SHT_STRTAB) && (sec.section_name == ".dynstr")){
            sh_dynstr_p = &raw[sec.section_offset];
            break;
        }
    }

    for(pwn::section sec : sections) {
        if((sec.section_type != SHT_SYMTAB) && (sec.section_type != SHT_DYNSYM))
            continue;
        size_t nsyms = sec.section_size / sizeof(Elf64_Sym);
        Elf64_Sym * syms_data = (Elf64_Sym *)&raw[sec.section_offset];

        for(int i=0; i<nsyms; ++i) {
            pwn::symbol s;
            s.symbol_value = syms_data[i].st_value;
            s.symbol_size = syms_data[i].st_size;
            s.symbol_type = syms_data[i].st_info;
            s.symbol_section = sec.section_name;
            
            if(sec.section_type == SHT_SYMTAB)
                s.symbol_name = std::string((char *)&sh_strtab_p[syms_data[i].st_name]);
            if(sec.section_type == SHT_DYNSYM)
                s.symbol_name = std::string((char *)&sh_dynstr_p[syms_data[i].st_name]);
            
            vect.push_back(s);
        }
    }
    return vect;
}

int64_t pwn::ELF::find_str_offset(const std::string &str) {
    int64_t idx = this->file.find(str);
    if(idx == std::string::npos) return -1;
    return idx;
}
