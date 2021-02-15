#define TOTAL_SECTION_HEADERS 8
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
