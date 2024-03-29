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

#ifndef PE_h
#define PE_H
#include "pefile_definitions.h"
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class PE {
  public:
    PE(const std::string &);
    void print_section_info(const std::string &);
    void print_sections();
    uint64_t &operator[](const std::string &);

  private:
    std::ifstream file;
    uint64_t file_size;
    image_dos_header dos_header;
    image_file_coff_header coff_header;
    image_optional_header_32bit optional_header_32bit;
    image_optional_header_64bit optional_header_64bit;
    uint32_t coffheader_offset;
    uint32_t pe_magicnum;
    uint16_t image_type;
    std::vector<section_header> section_headers;
    std::vector<uint32_t> export_address_table;
    std::vector<uint32_t> export_ordinal_table;
    std::vector<uint32_t> export_name_address_table;
    std::unordered_map<std::string, uint64_t> export_symbol_map;
    export_directory_table __export_directory_table;

    // return section header index by name
    int32_t get_section_header_by_name(const std::string &);
    void print_section_info_by_index(int32_t);
    uint32_t get_file_offset_of_data_directory(DATA_DIRECTORY);
    uint64_t get_virtual_address_data_directory(DATA_DIRECTORY);
    void parse_export_directory_table();
    uint32_t get_file_offset_from_rva(uint32_t);
};
#endif
