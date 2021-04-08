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

#define TOTAL_DATA_DIRECTORIES 0x10

enum SIGNATURE {
    IMAGE_DOS_SIGNATURE = 0x5a4d,
    IMAGE_OS2_SIGNATURE = 0x454e,
    IMAGE_OS2_SIGNATURE_LE = 0x454c,
    IMAGE_NT_SIGNATURE = 0x00004550
};

enum PE_MAGIC { PE_ROM_IMAGE = 0x107, PE_32BIT = 0x10b, PE_64BIT = 0x20b };

enum DATA_DIRECTORY {
    export_table_entry,
    import_table_entry,
    resource_table_entry,
    exception_table_entry,
    certificate_table_entry,
    base_relocation_entry,
    debug_entry,
    architecture_entry,
    global_ptr_entry,
    tls_table_entry,
    load_config_table_entry,
    bound_import_entry,
    iat_entry,
    delay_import_descriptor_entry,
    clr_runtime_header_entry,
    reserved_entry,
};

enum PE_SECTION_FLAG_CHARACTERISTICS {

    IMAGE_SCN_RESERVED_0001 = 0x1,
    IMAGE_SCN_RESERVED_0002 = 0x2,
    IMAGE_SCN_RESERVED_0004 = 0x4,
    IMAGE_SCN_TYPE_NO_PAD = 0x8,
    IMAGE_SCN_RESERVED_0010 = 0x10,
    IMAGE_SCN_CNT_CODE = 0x20,
    IMAGE_SCN_CNT_INITIALIZED_DATA = 0x40,
    IMAGE_SCN_CNT_UNINITIALIZED_DATA = 0x80,
    IMAGE_SCN_LNK_OTHER = 0x100,
    IMAGE_SCN_LNK_INFO = 0x200,
    IMAGE_SCN_RESERVED_0400 = 0x400,
    IMAGE_SCN_LNK_REMOVE = 0x800,
    IMAGE_SCN_LNK_COMDAT = 0x1000,
    IMAGE_SCN_GPREL = 0x8000,
    IMAGE_SCN_MEM_PURGEABLE = 0x10000,
    IMAGE_SCN_MEM_16BIT = 0x20000,
    IMAGE_SCN_MEM_LOCKED = 0x40000,
    IMAGE_SCN_MEM_PRELOAD = 0x80000,
    IMAGE_SCN_LNK_NRELOC_OVFL = 0x1000000,
    IMAGE_SCN_MEM_DISCARDABLE = 0x2000000,
    IMAGE_SCN_MEM_NOT_CACHED = 0x4000000,
    IMAGE_SCN_MEM_NOT_PAGED = 0x8000000,
    IMAGE_SCN_MEM_SHARED = 0x10000000,
    IMAGE_SCN_MEM_EXECUTE = 0x20000000,
    IMAGE_SCN_MEM_READ = 0x40000000,
    IMAGE_SCN_MEM_WRITE = 0x80000000
};

enum MACHINE {
    UNKNOWN = 0x0,
    AM33 = 0x1d3,
    AMD64 = 0x8664,
    ARM = 0x1c0,
    ARM64 = 0xaa64,
    ARMNT = 0x1c4,
    EBC = 0xebc,
    I386 = 0x14c,
    IA64 = 0x200,
    M32R = 0x9041,
    MIPS16 = 0x266,
    MIPSFPU = 0x366,
    MIPSFPU16 = 0x466,
    POWERPC = 0x1f0,
    POWERPCFP = 0x1f1,
    R4000 = 0x166,
    RISCV32 = 0x5032,
    RISCV64 = 0x5064,
    RISCV128 = 0x5128,
    SH3 = 0x1a2,
    SH3DSP = 0x1a3,
    SH4 = 0x1a6,
    SH5 = 0x1a8,
    THUMB = 0x1c2,
    WCEMIPSV2 = 0x169
};

struct image_dos_header {
    uint16_t e_magic;    // Magic number.
    uint16_t e_cblp;     // Bytes on last page of file.
    uint16_t e_cp;       // Pages in file.
    uint16_t e_crlc;     // Relocations.
    uint16_t e_cparhdr;  // Size of headers in paragraphs.
    uint16_t e_minalloc; // Minimum extra paragraphs needed.
    uint16_t e_maxalloc; // Maximim extra paragraphs needed.
    uint16_t e_ss;       // Initial (relative) SS value.
    uint16_t e_sp;       // Initial SP value.
    uint16_t e_csum;     // Checksum.
    uint16_t e_ip;       // Initial IP value.
    uint16_t e_cs;       // Initial (relative) CS value
    uint16_t e_lfarlc;   // File address of relocation table
    uint16_t e_ovno;     // Overlay number
    char e_res[8];       // Reserved words
    uint16_t e_oemid;    // OEM identifier (for e_oeminfo)
    uint16_t e_oeminfo;  // OEM information; e_oemid specific
    char e_res2[20];     // Reserved words
    uint32_t e_lfanew;   // File address of new exe uint32_t
};

struct image_file_coff_header {
    uint32_t signature;
    uint16_t machine;
    uint16_t n_sections;
    uint32_t time_date_stamp;
    uint32_t pointer_symbol_table;
    uint32_t n_symbols;
    uint16_t optional_header_size;
    uint16_t characteristics;
};

struct pe_data_directory_entry {
    uint32_t virtual_address;
    uint32_t size;
};

struct image_optional_header_32bit {
    // Standard fields
    uint16_t magic;
    uint8_t major_linker_version;
    uint8_t minor_linker_version;
    uint32_t code_size;
    uint32_t size_of_initialized_data;
    uint32_t size_of_uninitialized_data;
    uint32_t address_entrypoint;
    uint32_t code_base;
    uint32_t data_base;
    // Nt additional fields
    uint32_t image_base;
    uint32_t section_alignment;
    uint32_t file_alignment;
    uint16_t major_operating_system_version;
    uint16_t minor_operating_system_version;
    uint16_t major_image_version;
    uint16_t minor_image_version;
    uint16_t major_subsystem_version;
    uint16_t minor_subsystem_version;
    uint32_t win32_version_value;
    uint32_t image_size;
    uint32_t header_size;
    uint32_t checksum;
    uint16_t subsystem;
    uint16_t dll_characteristics;
    uint32_t size_of_stack_reserve;
    uint32_t size_of_stack_commit;
    uint32_t size_of_heap_reserve;
    uint32_t size_of_heap_commit;
    uint32_t loaderflags;
    uint32_t n_rva_sizes;
    pe_data_directory_entry data_directories[TOTAL_DATA_DIRECTORIES];
};

struct image_optional_header_64bit {
    // Standard fields
    uint16_t magic;
    uint8_t major_linker_version;
    uint8_t minor_linker_version;
    uint32_t code_size;
    uint32_t size_of_initialized_data;
    uint32_t size_of_uninitialized_data;
    uint32_t address_entrypoint;
    uint32_t code_base;
    // Nt additional fields
    uint64_t image_base;
    uint32_t section_alignment;
    uint32_t file_alignment;
    uint16_t major_operating_system_version;
    uint16_t minor_operating_system_version;
    uint16_t major_image_version;
    uint16_t minor_image_version;
    uint16_t major_subsystem_version;
    uint16_t minor_subsystem_version;
    uint32_t win32_version_value;
    uint32_t image_size;
    uint32_t header_size;
    uint32_t checksum;
    uint16_t subsystem;
    uint16_t dll_characteristics;
    uint64_t size_of_stack_reserve;
    uint64_t size_of_stack_commit;
    uint64_t size_of_heap_reserve;
    uint64_t size_of_heap_commit;
    uint32_t loaderflags;
    uint32_t n_rva_sizes;
    pe_data_directory_entry data_directories[TOTAL_DATA_DIRECTORIES];
};

struct section_header {
    char name[8];
    uint32_t virtual_size;
    uint32_t virtual_address;
    uint32_t size_of_raw_data;
    uint32_t pointer_raw_data;
    uint32_t pointer_relocations;
    uint32_t pointer_line_numbers;
    uint16_t number_of_relocations;
    uint16_t number_of_line_numbers;
    uint32_t characteristics;
};

struct export_directory_table {
    uint32_t export_flags;
    uint32_t time_date_stamp;
    uint16_t major_version;
    uint16_t minor_version;
    uint32_t name_rva;
    uint32_t ordinal_base;
    uint32_t address_table_entries;
    uint32_t number_of_name_pointers;
    uint32_t export_address_table_rva;
    uint32_t name_pointer_rva;
    uint32_t ordinal_table_rva;
};
