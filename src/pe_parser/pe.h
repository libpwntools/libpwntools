#ifndef PE_h
#define PE_H
#include "pefile_definitions.h"
#include <fstream>
#include <iostream>
#include <string>

class PE {
  public:
    PE(const std::string &);

  private:
    std::ifstream file;
    uint64_t file_size;
    image_dos_header dos_header;
    image_file_coff_header coff_header;
    image_optional_header optional_header;
    uint32_t coffheader_offset;
    uint16_t image_type;
    uint16_t number_of_sections;
};
#endif
