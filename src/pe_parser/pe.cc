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
    this->number_of_sections = this->coff_header.n_sections;
}
