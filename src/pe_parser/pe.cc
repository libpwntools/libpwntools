#include "pe.h"

PE::PE(const std::string &filename) {
    this->file = std::ifstream(filename.c_str(), std::ifstream::binary);
    if (!this->file) {
        std::cout << "Error opening file " << filename << std::endl;
        exit(1);
    }
    /* Get file size */
    this->file.seekg(0, this->file.end);
    this->file_size = this->file.tellg();
    this->file.seekg(0, this->file.beg);

    /* Parse dos header */
    this->file.read((char *)&this->dos_header, sizeof(image_dos_header));
    this->coffheader_offset = this->dos_header.e_lfanew;

    /* Check magic number */
    if (this->dos_header.e_magic != IMAGE_DOS_SIGNATURE) {
        std::cout << "Unknown Format " << std::endl;
        return;
    }
    /* Get file offset to PE header */
    if (this->coffheader_offset > this->file_size) {
        std::cout << "Cannot relocate PE header " << std::endl;
        return;
    }
    /* Parse PE header */
    this->file.seekg(this->coffheader_offset);
    this->file.read((char *)&this->coff_header, sizeof(image_file_coff_header));
    uint32_t type = this->coff_header.signature;

    if (type == IMAGE_DOS_SIGNATURE || type == IMAGE_OS2_SIGNATURE ||
        type == IMAGE_OS2_SIGNATURE_LE || type == IMAGE_NT_SIGNATURE) {
        this->image_type = type;
    } else {
        std::cout << "Unknown Format " << std::endl;
        return;
    }
    /* Parse the Option Header */
    uint16_t magic_num;
    this->file.read((char *)&magic_num, 2);
    if (magic_num == PE_32BIT) {
        this->file.read((char *)&this->optional_header_32bit + 2,
                        sizeof(image_optional_header_32bit) - 2);
        this->optional_header_32bit.magic = this->pe_magicnum = magic_num;
        this->optional_header_64bit = {0};
    } else if (magic_num == PE_64BIT) {
        this->file.read((char *)&this->optional_header_64bit + 2,
                        sizeof(image_optional_header_64bit) - 2);
        this->optional_header_64bit.magic = this->pe_magicnum = magic_num;
        this->optional_header_32bit = {0};
    } else {
        std::cout << "Unknown format " << std::endl;
        return;
    }
    /* Parse Section Headers */
    for (int i = 0; i < TOTAL_SECTION_HEADERS; i++) {
        this->file.read((char *)&this->section_headers[i],
                        sizeof(section_header));
    }

    std::cout << "0x" << std::hex
              << this->get_file_offset_of_data_directory(debug_entry)
              << std::endl;
}

void PE::print_section_info(const std::string &section_name) {
    int32_t section_idx = this->get_section_header_by_name(section_name);
    if (section_idx != -1) {
        this->print_section_info_by_index(section_idx);
    } else {
        std::cout << "Invalid Section ( Not found )" << std::endl;
    }
}

void PE::print_section_info_by_index(int32_t idx) {
    std::cout << "Section Name: " << section_headers[idx].name << std::endl;
    std::cout << "Virtual Size: 0x" << std::hex
              << section_headers[idx].virtual_size << std::endl;
    std::cout << "Virtual Address: 0x" << std::hex
              << section_headers[idx].virtual_address << std::endl;
    std::cout << "Size of Raw Data: 0x" << std::hex
              << section_headers[idx].size_of_raw_data << std::endl;
    std::cout << "Pointer to Raw Data: 0x" << std::hex
              << section_headers[idx].pointer_raw_data << std::endl;
    std::cout << "Pointer to line numbers: 0x" << std::hex
              << section_headers[idx].pointer_line_numbers << std::endl;
    std::cout << "Number of Relocations: 0x" << std::hex
              << section_headers[idx].number_of_relocations << std::endl;
    std::cout << "Number of Line numbers: 0x" << std::hex
              << section_headers[idx].number_of_line_numbers << std::endl;
    std::cout << "Characteristics of Section: 0x" << std::hex
              << section_headers[idx].characteristics << std::endl;
}

int32_t PE::get_section_header_by_name(const std::string &section_name) {
    for (int i = 0; i < TOTAL_SECTION_HEADERS; i++)
        if (!strcmp(section_name.c_str(), this->section_headers[i].name)) {
            return i;
        }
    return -1;
}

int64_t PE::get_file_offset_of_data_directory(DATA_DIRECTORY dir) {
    if (dir >= TOTAL_DATA_DIRECTORIES) {
        return -1;
    }
    uint32_t directory_virtual_addr{0};
    if (this->pe_magicnum == PE_32BIT) {
        directory_virtual_addr =
            this->optional_header_32bit.data_directories[dir].virtual_address;
    } else if (this->pe_magicnum == PE_64BIT) {
        directory_virtual_addr =
            this->optional_header_64bit.data_directories[dir].virtual_address;
    } else {
        return -1;
    }
    int i = 0;
    for (i; i < TOTAL_SECTION_HEADERS; i++) {
        if (this->section_headers[i].virtual_address <=
                directory_virtual_addr &&
            this->section_headers[i].virtual_address +
                    this->section_headers[i].size_of_raw_data >
                directory_virtual_addr)
            break;
    }
    if (i >= TOTAL_DATA_DIRECTORIES) {
        return -1;
    }

    return (this->section_headers[i].pointer_raw_data) +
           (directory_virtual_addr - this->section_headers[i].virtual_address);
}
