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
#ifndef TOTAL_SECTION_HEADERS
#define TOTAL_SECTION_HEADERS this->coff_header.n_sections
#endif
    section_header tmp;
    std::memset(&tmp, 0, sizeof(section_headers));
    /* Parse N Section Headers */
    for (int i = 0; i < TOTAL_SECTION_HEADERS; i++) {
        this->file.read((char *)&tmp, sizeof(section_header));
        this->section_headers.push_back(tmp);
    }
    this->parse_export_directory_table();
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
    std::cout << "Section Name: " << this->section_headers.at(idx).name
              << std::endl;
    std::cout << "Virtual Size: 0x" << std::hex
              << this->section_headers.at(idx).virtual_size << std::endl;
    std::cout << "Virtual Address: 0x" << std::hex
              << this->section_headers.at(idx).virtual_address << std::endl;
    std::cout << "Size of Raw Data: 0x" << std::hex
              << this->section_headers.at(idx).size_of_raw_data << std::endl;
    std::cout << "Pointer to Raw Data: 0x" << std::hex
              << this->section_headers.at(idx).pointer_raw_data << std::endl;
    std::cout << "Pointer to line numbers: 0x" << std::hex
              << this->section_headers.at(idx).pointer_line_numbers
              << std::endl;
    std::cout << "Number of Relocations: 0x" << std::hex
              << this->section_headers.at(idx).number_of_relocations
              << std::endl;
    std::cout << "Number of Line numbers: 0x" << std::hex
              << this->section_headers.at(idx).number_of_line_numbers
              << std::endl;
    std::cout << "Characteristics of Section: 0x" << std::hex
              << this->section_headers.at(idx).characteristics << std::endl;
}

int32_t PE::get_section_header_by_name(const std::string &section_name) {
    for (int i = 0; i < TOTAL_SECTION_HEADERS; i++)
        if (!strcmp(section_name.c_str(), this->section_headers.at(i).name)) {
            return i;
        }
    return -1;
}

uint32_t PE::get_file_offset_of_data_directory(DATA_DIRECTORY dir) {
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
    return this->get_file_offset_from_rva(directory_virtual_addr);
}

void PE::print_sections() {
    int j = 0;
    for (auto i : this->section_headers) {
        std::cout << i.name << std::endl;
        this->print_section_info_by_index(j++);
    }
}

uint64_t PE::get_virtual_address_data_directory(DATA_DIRECTORY dir) {
    if (dir >= TOTAL_DATA_DIRECTORIES) {
        return -1;
    }
    if (this->pe_magicnum == PE_32BIT) {
        return this->optional_header_32bit.image_base +
               this->optional_header_32bit.data_directories[dir]
                   .virtual_address;
    } else if (this->pe_magicnum == PE_64BIT) {
        return this->optional_header_64bit.image_base +
               this->optional_header_64bit.data_directories[dir]
                   .virtual_address;
    } else {
        return -1;
    }
}

void PE::parse_export_directory_table() {

    uint32_t export_dtable_file_offset = 0;
    if (this->get_virtual_address_data_directory(export_table_entry) <= 0) {
        std::cout << "Error parsing Export directory table " << std::endl;
        exit(1);
    } else {
        /* Seek to export directory table file offset */
        export_dtable_file_offset =
            this->get_file_offset_of_data_directory(export_table_entry);
        this->file.seekg(export_dtable_file_offset);
        this->file.read((char *)&this->__export_directory_table,
                        sizeof(export_directory_table));

        /* Parse address table entries */
        uint32_t tmp = 0;
        uint32_t i;
        uint32_t address_table_file_offset = this->get_file_offset_from_rva(
            this->__export_directory_table.export_address_table_rva);
        if (address_table_file_offset == -1) {
            std::cout << "Error parsing export directory table" << std::endl;
            exit(1);
        }
#ifndef N_EXPORT_TABLE_SYMBOLS
#define N_EXPORT_TABLE_SYMBOLS                                                 \
    this->__export_directory_table.address_table_entries
#endif
        this->file.seekg(address_table_file_offset);
        for (i = 0; i < N_EXPORT_TABLE_SYMBOLS; i++) {
            this->file.read((char *)&tmp, sizeof(uint32_t));
            this->export_address_table.push_back(tmp);
        }

        /* Parse name pointer addresses */
        uint32_t name_table_pointer = this->get_file_offset_from_rva(
            this->__export_directory_table.name_pointer_rva);
        if (name_table_pointer == -1) {
            std::cout << "Error parsing name table " << std::endl;
            exit(1);
        }
        this->file.seekg(name_table_pointer);
        for (i = 0; i < N_EXPORT_TABLE_SYMBOLS; i++) {
            this->file.read((char *)&tmp, sizeof(uint32_t));
            this->export_name_address_table.push_back(tmp);
        }

        /* Parse the Ordinal table */
        uint32_t ordinal_table_raw_offset = this->get_file_offset_from_rva(
            this->__export_directory_table.ordinal_table_rva);
        if (ordinal_table_raw_offset == -1) {
            std::cout << "Error Parsing name table " << std::endl;
            exit(1);
        }
        tmp = 0;
        this->file.seekg(ordinal_table_raw_offset);
        for (i = 0; i < N_EXPORT_TABLE_SYMBOLS; i++) {
            this->file.read((char *)&tmp, sizeof(uint16_t));
            this->export_ordinal_table.push_back(tmp);
        }

        /* Parse symbol names */
        uint32_t symbol_string_raw_offset = 0;
        uint32_t symbol_virtual_addr = 0;
        std::string symbol_name;
        i = 0;
        for (auto j : this->export_ordinal_table) {
            symbol_virtual_addr = this->export_address_table.at(j);
            symbol_string_raw_offset = this->get_file_offset_from_rva(
                this->export_name_address_table.at(i++));
            this->file.seekg(symbol_string_raw_offset);
            std::getline(this->file, symbol_name, '\x00');
            this->export_symbol_map[symbol_name.c_str()] = symbol_virtual_addr;
        }
    }
}

/* Get file raw offset from virtual address */
uint32_t PE::get_file_offset_from_rva(uint32_t rva) {
    for (auto i : this->section_headers) {
        if (i.virtual_address <= rva &&
            i.virtual_address + i.size_of_raw_data > rva) {
            return i.pointer_raw_data + (rva - i.virtual_address);
        }
    }
    return -1;
}

uint64_t &PE::operator[](const std::string &symbol_name) {
    if (this->export_symbol_map.find(symbol_name) ==
        this->export_symbol_map.end()) {
        std::cout << "Symbol Not found " << std::endl;
        exit(1);
    }
    return this->export_symbol_map[symbol_name];
}
