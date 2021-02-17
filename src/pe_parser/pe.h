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
