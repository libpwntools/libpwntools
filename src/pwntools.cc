#include <pwntools>

void pwn::pause() {
    std::cout << "(pause) Press enter to release." << std::endl;
    getchar();
}

std::string pwn::pack(uint64_t n) {
    return std::string((char *)&n, 8);
}

std::string pwn::pack(uint32_t n) {
    return std::string((char *)&n, 4);
}

std::string pwn::pack(int64_t n) {
    return std::string((char *)&n, 8);
}

std::string pwn::pack(unsigned long long n) {
    return std::string((char *)&n, 8);
}

std::string pwn::pack(long long n) {
    return std::string((char *)&n, 8);
}

std::string pwn::pack(int32_t n) {
    return std::string((char *)&n, 4);
}

std::string pwn::pack(const std::string &s) {
    return s;
}

std::string pwn::p64(uint64_t n) {
    return pwn::pack(n);
}

std::string pwn::p32(uint32_t n) {
    return pwn::pack(n);
}

uint64_t pwn::u64(const std::string &buf) {
    assert(buf.length() == 8);
    return *(uint64_t *)buf.c_str();
}

uint32_t pwn::u32(const std::string &buf) {
    assert(buf.length() == 4);
    return *(uint32_t *)buf.c_str();
}

pwn::file::file() {
	this->fp = new FILE;
	std::memset((void *)fp, 0, sizeof(FILE) );
}

std::string pwn::file::construct() {
    return std::string((char *)this->fp, sizeof(FILE) );
}

pwn::file::~file() {
	delete this->fp;
}

pwn::SigReturnFrame::SigReturnFrame(std::string kern, std::string file_arch) {
    this->gs_32 = 0;
    this->fs_32 = 0;
    this->es_32 = 0;
    this->ds_32 = 0;
    this->edi_32 = 0;
    this->esi_32 = 0;
    this->ebp_32 = 0;
    this->esp_32 = 0;
    this->ebx_32 = 0;
    this->edx_32 = 0;
    this->ecx_32 = 0;
    this->eax_32 = 0;
    this->trapno_32 = 0;
    this->err_32 = 0;
    this->eip_32 = 0;
    this->cs_32 = 0x23; // default 
    this->eflags_32 = 0;
    this->esp_at_signal_32 = 0;
    this->ss_32 = 0x2b; // default
    this->fpstate_32 = 0;

    this->uc_flags = 0;
    this->uc = 0;
    this->uc_stack_ss_sp = 0;
    this->uc_stack_ss_flags = 0;
    this->uc_stack_ss_size = 0;
    this->r8 = 0;
    this->r9 = 0;
    this->r10 = 0;
    this->r11 = 0;
    this->r12 = 0;
    this->r13 = 0;
    this->r14 = 0;
    this->r15 = 0;
    this->rdi = 0;
    this->rsi = 0;
    this->rbp = 0;
    this->rbx = 0;
    this->rdx = 0;
    this->rax = 0;
    this->rcx = 0;
    this->rsp = 0;
    this->rip = 0;
    this->eflags_64 = 0;
    this->csgsfs = 0x33; // default
    this->err_64 = 0;
    this->trapno_64 = 0;
    this->oldmask_64 = 0;
    this->cr2_64 = 0;
    this->fpstate_64 = 0;
    this->reserved = 0;
    this->sigmask = 0;

	this->arch = file_arch;
	this->kernel = kernel;
	if(this->kernel == "i386" ) {
		this->cs_32 = 0x73;
		this->ss_32 = 0x7b;
	}
}

std::string pwn::SigReturnFrame::construct_frame() {
	if (this->kernel == "amd64") {
		return pwn::flat(this->uc_flags,this->uc,this->uc_stack_ss_sp,this->uc_stack_ss_flags,this->uc_stack_ss_size,
		this->r8,this->r9,this->r10,this->r11,this->r12,this->r13,this->r14,this->r15,this->rdi,
		this->rsi,this->rbp,this->rbx,this->rdx,this->rax,this->rcx,this->rsp,this->rip,this->eflags_64,
		this->csgsfs,this->err_32,this->trapno_64, this->oldmask_64, this->cr2_64, this->fpstate_64,
		this->reserved, this->sigmask);	
	}

	else if (this->arch == "i386") {
		return pwn::flat(this->gs_32, this->fs_32, this->es_32, this->ds_32, this->edi_32, this->esi_32,
		this->ebp_32, this->esp_32, this->ebx_32, this->edx_32, this->ecx_32, this->eax_32, this->trapno_32,
		this->err_32, this->eip_32, this->cs_32, this->eflags_32, this->esp_at_signal_32, this->ss_32, this->fpstate_32);
	}

	else {
		std::cout << "Supported archs (amd64 / i386). " << std::endl;
		exit(0);
	}
}
