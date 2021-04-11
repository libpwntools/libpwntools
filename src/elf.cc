/*
BSD 2-Clause License

Copyright (c) 2021, libpwntools
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <elf.h>
#include <fcntl.h>
#include <libpwntools/pwnelf.h>
#include <libpwntools/utils.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <cassert>
#include <iostream>

pwn::ELF::ELF(const std::string &filename) {
    int fd = open(filename.c_str(), O_RDONLY);
    assert(fd > 0);
    struct stat st;

    stat(filename.c_str(), &st);
    unsigned char *tmp = (unsigned char *)malloc(st.st_size);
    read(fd, tmp, st.st_size);
    close(fd);

    this->file = std::string((const char *)tmp, st.st_size);
    free(tmp);
    Elf64_Ehdr *elf_header = (Elf64_Ehdr *)this->file.c_str();
    assert(elf_header->e_ident[EI_CLASS] == ELFCLASS64);

    std::vector<pwn::symbol> syms = this->parse_symbols();
    for (pwn::symbol sym : syms) {
        std::string sym_name = sym.symbol_name;
        uint64_t sym_value = sym.symbol_value;
        if (sym_name.length() && sym_value)
            this->sym_map[sym_name] = sym_value;
    }
}

uint64_t &pwn::ELF::operator[](const std::string &sym_name) {
    if (this->sym_map.find(sym_name) == this->sym_map.end())
        throw std::runtime_error("Symbol " + sym_name + " not found");
    return this->sym_map[sym_name];
}

std::vector<pwn::section> pwn::ELF::parse_sections() {
    uint8_t *raw = (uint8_t *)this->file.c_str();
    pwn::check_bounds cb((uintptr_t)raw, this->file.size());
    Elf64_Ehdr *elf_header = (Elf64_Ehdr *)raw;
    Elf64_Shdr *section_header =
        cb.check((Elf64_Shdr *)&raw[elf_header->e_shoff]);

    size_t shnum = elf_header->e_shnum;
    Elf64_Shdr *sh_strtab = cb.check(&section_header[elf_header->e_shstrndx]);
    uint8_t *sh_strtab_p = cb.check(&raw[sh_strtab->sh_offset]);

    std::vector<pwn::section> vect;
    for (int i = 0; i < shnum; ++i) {
        pwn::section s;
        s.section_name = std::string((char *)(cb.check(
            &sh_strtab_p[*cb.check(&section_header[i].sh_name)])));
        s.section_type = *cb.check(&section_header[i].sh_type);
        s.section_offset = *cb.check(&section_header[i].sh_offset);
        s.section_address = *cb.check(&section_header[i].sh_addr);
        s.section_size = *cb.check(&section_header[i].sh_size);
        s.section_entsize = *cb.check(&section_header[i].sh_entsize);
        s.section_addr_align = *cb.check(&section_header[i].sh_addralign);
        vect.push_back(s);
    }
    return vect;
}

std::vector<pwn::symbol> pwn::ELF::parse_symbols() {
    uint8_t *raw = (uint8_t *)this->file.c_str();
    pwn::check_bounds cb((uintptr_t)raw, this->file.size());
    std::vector<pwn::section> sections = this->parse_sections();
    std::vector<pwn::symbol> vect;

    uint8_t *sh_strtab_p = nullptr;
    for (pwn::section sec : sections) {
        if ((sec.section_type == SHT_STRTAB) &&
            (sec.section_name == ".strtab")) {
            sh_strtab_p = cb.check(&raw[sec.section_offset]);
            break;
        }
    }

    uint8_t *sh_dynstr_p = nullptr;
    for (pwn::section sec : sections) {
        if ((sec.section_type == SHT_STRTAB) &&
            (sec.section_name == ".dynstr")) {
            sh_dynstr_p = cb.check(&raw[sec.section_offset]);
            break;
        }
    }

    for (pwn::section sec : sections) {
        if ((sec.section_type != SHT_SYMTAB) &&
            (sec.section_type != SHT_DYNSYM))
            continue;
        size_t nsyms = sec.section_size / sizeof(Elf64_Sym);
        Elf64_Sym *syms_data = cb.check((Elf64_Sym *)&raw[sec.section_offset]);

        for (int i = 0; i < nsyms; ++i) {
            pwn::symbol s;
            s.symbol_value = *cb.check(&syms_data[i].st_value);
            s.symbol_size = *cb.check(&syms_data[i].st_size);
            s.symbol_type = *cb.check(&syms_data[i].st_info);
            s.symbol_section = sec.section_name;

            if (sec.section_type == SHT_SYMTAB)
                s.symbol_name = std::string(cb.check(
                    (char *)&sh_strtab_p[*cb.check(&syms_data[i].st_name)]));
            if (sec.section_type == SHT_DYNSYM)
                s.symbol_name = std::string(cb.check(
                    (char *)&sh_dynstr_p[*cb.check(&syms_data[i].st_name)]));

            vect.push_back(s);
        }
    }
    return vect;
}

int64_t pwn::ELF::find_str_offset(const std::string &str) {
    int64_t idx = this->file.find(str);
    if (idx == std::string::npos)
        return -1;
    return idx;
}
