#ifndef PE_h
#define PE_H
#include "pe_definitions.h"
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

class PE {
  public:
    PE(const std::string &);
    void print_section_info(const std::string &);

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
    section_header section_headers[8];

    // return section header index by name
    int32_t get_section_header_by_name(const std::string &);
    void print_section_info_by_index(int32_t);
    int64_t get_file_offset_of_data_directory(DATA_DIRECTORY);
};
#endif
