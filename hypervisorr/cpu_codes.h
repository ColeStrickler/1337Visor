#pragma once

// TYPEDEFS
#define BYTE UINT8
#define WORD UINT16
#define DWORD UINT32
#define LPDWORD DWORD*


// SIZE DEFINITIONS
#define MSRPERM_MAP_SIZE						(PAGE_SIZE * 2)


// IA32e EXTENDED FEATURE ENABLE REGISTER
#define IA32_EFER								0xC0000080
#define IA32_EFER_SVME							(1 << 12)


// CPUID CODES
#define CPUID_VENDOR_STRING						0x00000000
#define CPUID_PROCESSOR_FEATURE_IDENTIFIERS		0x80000001
#define CPUID_SVM_FEATURE_ID					0x8000000A
#define CPUID_MICROSOFT_HV_INTERFACE			0x40000001
#define CPUID_HYPERVISOR_VENDOR_ID				0x40000000
#define CPUID_EXIT_VIRTUAL_SESSION				0x13371337

// FEATURE ID BIT SELECTORS
#define CPUID_FN8000_0001_ECX_SVM				(1 << 2)
#define CPUID_FN8000_000A_EDX_SVML				(1 << 2)
#define CPUID_FN8000_000A_EDX_NP				 1
#define CPUID_FN0000_0001_ECX_HV_PRESENT		(1 << 31)


// MSR CODES
#define MSR_VM_CR								0xC0010114
#define MSR_VM_HSAVE_PA							0xC0010117		// Holds "host state-save" area, VMRUN will save host processor state info address held by this MSR
#define MSR_PAT									0x00000277		// Page Attribute Table Register



// MSR BIT SELECTORS
#define MSR_VM_CR_SVMDIS						(1 >> 4)


// MSR PERMISSION MAP BIT SELECTORS





// Copied from https://github.com/medievalghoul/krakensvm-mg/blob/main/krakensvm/inc/hv_util.hpp
enum VMEXIT : INT16
{
	// VMEXIT_CR[0–15]_READ; read of CR 0 through 15, respectively
	_CR0_READ = 0x0,
	_CR1_READ = 0x1,
	_CR2_READ = 0x2,
	_CR3_READ = 0x3,
	_CR4_READ = 0x4,
	_CR5_READ = 0x5,
	_CR6_READ = 0x6,
	_CR7_READ = 0x7,
	_CR8_READ = 0x8,
	_CR9_READ = 0x9,
	_CR10_READ = 0xa,
	_CR11_READ = 0xb,
	_CR12_READ = 0xc,
	_CR13_READ = 0xd,
	_CR14_READ = 0xe,
	_CR15_READ = 0xf,

	// VMEXIT_CR[0–15]_WRITE; write of CR 0 through 15, respectively
	_CR0_WRITE = 0x10,
	_CR1_WRITE = 0x11,
	_CR2_WRITE = 0x12,
	_CR3_WRITE = 0x13,
	_CR4_WRITE = 0x14,
	_CR5_WRITE = 0x15,
	_CR6_WRITE = 0x16,
	_CR7_WRITE = 0x17,
	_CR8_WRITE = 0x18,
	_CR9_WRITE = 0x19,
	_CR10_WRITE = 0x1a,
	_CR11_WRITE = 0x1b,
	_CR12_WRITE = 0x1c,
	_CR13_WRITE = 0x1d,
	_CR14_WRITE = 0x1e,
	_CR15_WRITE = 0x1f,

	// VMEXIT_DR[0–15]_READ; read of DR 0 through 15, respectively
	_DR0_READ = 0x20,
	_DR1_READ = 0x21,
	_DR2_READ = 0x22,
	_DR3_READ = 0x23,
	_DR4_READ = 0x24,
	_DR5_READ = 0x25,
	_DR6_READ = 0x26,
	_DR7_READ = 0x27,
	_DR8_READ = 0x28,
	_DR9_READ = 0x29,
	_DR10_READ = 0x2a,
	_DR11_READ = 0x2b,
	_DR12_READ = 0x2c,
	_DR13_READ = 0x2d,
	_DR14_READ = 0x2e,
	_DR15_READ = 0x2f,

	// VMEXIT_DR[0–15]_WRITE; write of DR 0 through 15, respectively
	_DR0_WRITE = 0x30,
	_DR1_WRITE = 0x31,
	_DR2_WRITE = 0x32,
	_DR3_WRITE = 0x33,
	_DR4_WRITE = 0x34,
	_DR5_WRITE = 0x35,
	_DR6_WRITE = 0x36,
	_DR7_WRITE = 0x37,
	_DR8_WRITE = 0x38,
	_DR9_WRITE = 0x39,
	_DR10_WRITE = 0x3a,
	_DR11_WRITE = 0x3b,
	_DR12_WRITE = 0x3c,
	_DR13_WRITE = 0x3d,
	_DR14_WRITE = 0x3e,
	_DR15_WRITE = 0x3f,

	// VMEXIT_EXCP[0–31]; exception vector 0–31, respectively
	_EXCP0_WRITE = 0x40,
	_EXCP1_WRITE = 0x41,
	_EXCP2_WRITE = 0x42,
	_EXCP3_WRITE = 0x43,
	_EXCP4_WRITE = 0x44,
	_EXCP5_WRITE = 0x45,
	_EXCP6_WRITE = 0x46,
	_EXCP7_WRITE = 0x47,
	_EXCP8_WRITE = 0x48,
	_EXCP9_WRITE = 0x49,
	_EXCP10_WRITE = 0x4a,
	_EXCP11_WRITE = 0x4b,
	_EXCP12_WRITE = 0x4c,
	_EXCP13_WRITE = 0x4d,
	_EXCP14_WRITE = 0x4e,
	_EXCP15_WRITE = 0x4f,
	_EXCP16_WRITE = 0x50,
	_EXCP17_WRITE = 0x51,
	_EXCP18_WRITE = 0x52,
	_EXCP19_WRITE = 0x53,
	_EXCP20_WRITE = 0x54,
	_EXCP21_WRITE = 0x55,
	_EXCP22_WRITE = 0x56,
	_EXCP23_WRITE = 0x57,
	_EXCP24_WRITE = 0x58,
	_EXCP25_WRITE = 0x59,
	_EXCP26_WRITE = 0x5a,
	_EXCP27_WRITE = 0x5b,
	_EXCP28_WRITE = 0x5c,
	_EXCP29_WRITE = 0x5d,
	_EXCP30_WRITE = 0x5e,
	_EXCP31_WRITE = 0x5f,

	// VMEXIT_INTR; physical INTR (maskable interrupt)
	_INTR = 0x60,

	// VMEXIT_NMI; physical NM
	_NMI = 0x61,

	// VMEXIT_SMI; physical SMI (the EXITINFO1 field provides more information)
	_SMI = 0x62,

	// VMEXIT_INIT; physical INIT
	_INIT = 0x63,

	// VMEXIT_VINTR; virtual INTR
	_VINTR = 0x64,

	// VMEXIT_CR0_SEL_WRITE; write of CR0 that changed any bits
	// other than CR0.TS or CR0.MP
	_CR0_SEL_WRITE = 0x65,

	// VMEXIT_IDTR_READ; read of IDTR
	_IDTR_READ = 0x66,

	// VMEXIT_GDTR_READ; read of GDTR
	_GDTR_READ = 0x67,

	// VMEXIT_LDTR_READ; read of LDTR
	_LDTR_READ = 0x68,

	// VMEXIT_TR_READ; read of TR
	_TR_READ = 0x69,

	// VMEXIT_IDTR_WRITE; write of IDTR
	_IDTR_WRITE = 0x6A,

	// VMEXIT_GDTR_WRITE; write of GDTR
	_GDTR_WRITE = 0x6B,

	// VMEXIT_LDTR_WRITE; write of LDTR
	_LDTR_WRITE = 0x6C,

	// VMEXIT_TR_WRITE; write of TR
	_TR_WRITE = 0x6D,

	// VMEXIT_RDTSC; RDTSC instruction
	_RDTSC = 0x6E,

	// VMEXIT_RDPMC; RDPMC instruction
	_RDPMC = 0x6F,

	// VMEXIT_PUSHF; PUSHF instruction
	_PUSHF = 0x70,

	// VMEXIT_POPF; POPF instruction
	_POPF = 0x71,

	// VMEXIT_CPUID; CPUID instruction
	_CPUID = 0x72,

	// VMEXIT_RSM; RSM instruction
	_RSM = 0x73,

	// VMEXIT_IRET; IRET instruction
	_IRET = 0x74,

	// VMEXIT_SWINT; software interrupt (INTn instructions)
	_SWINT = 0x75,

	// VMEXIT_INVD; INVD instruction
	_INVD = 0x76,

	// VMEXIT_PAUSE; PAUSE instruction
	_PAUSE = 0x77,

	// VMEXIT_HLT; HLT instruction
	_HLT = 0x78,

	// VMEXIT_INVLPG; INVLPG instructions
	_INVLPG = 0x79,

	// VMEXIT_INVLPGA; INVLPGA instruction
	_INVLPGA = 0x7A,

	// VMEXIT_IOIO IN or OUT accessing protected port (the
	// EXITINFO1 field provides more information)
	_IOIO = 0x7B,

	// VMEXIT_MSR; RDMSR or WRMSR access to protected MSR
	_MSR = 0x7C,

	// VMEXIT_TASK_SWITCH; task switch
	_TASK_SWITCH = 0x7D,

	// VMEXIT_FERR_FREEZE; FP legacy handling enabled, and processor is frozen in an
	// x87 / mmx instruction waiting for an interrupt
	_FERR_FREEZE = 0x7E,

	// VMEXIT_SHUTDOWN Shutdown
	_SHUTDOWN = 0x7F,

	// VMEXIT_VMRUN; VMRUN instruction
	_VMRUN = 0x80,

	// VMEXIT_VMMCALL; VMMCALL instruction
	_VMMCALL = 0x81,

	// VMEXIT_VMLOAD; VMLOAD instruction
	_VMLOAD = 0x82,

	// VMEXIT_VMSAVE; VMSAVE instruction
	_VMSAVE = 0x83,

	// VMEXIT_STGI; STGI instruction
	_STGI = 0x84,

	// VMEXIT_CLGI; CLGI instruction
	_CLGI = 0x85,

	// VMEXIT_SKINIT; SKINIT instruction
	_SKINIT = 0x86,

	// VMEXIT_RDTSCP; RDTSCP instruction
	_RDTSCP = 0x87,

	// VMEXIT_ICEBP; ICEBP instruction
	_ICEBP = 0x88,

	// VMEXIT_WBINVD; WBINVD or WBNOINVD instruction
	_WBINVD = 0x89,

	// VMEXIT_MONITOR; MONITOR or MONITORX instruction
	_MONITOR = 0x8A,

	// VMEXIT_MWAIT; MWAIT or MWAITX instruction
	_MWAIT = 0x8B,

	// VMEXIT_MWAIT_CONDITIONAL MWAIT or MWAITX instruction,
	// if monitor hardware is armed.
	_MWAIT_CONDITIONAL = 0x8C,

	// VMEXIT_RDPRU; RDPRU instruction
	_RDPRU = 0x8D,

	// VMEXIT_XSETBV; XSETBV instruction
	_XSETBV = 0x8E,

	// VMEXIT_EFER_WRITE_TRAP Write of EFER MSR (occurs after guest instruction
	// finishes)
	_EFER_WRITE_TRAP = 0x8F,

	// VMEXIT_CR[0-15]_WRITE_TRAP Write of CR0-15, respectively (occurs after
	// guest instruction finishes)

	_CR0_WRITE_TRAP = 0x90,
	_CR1_WRITE_TRAP = 0x91,
	_CR2_WRITE_TRAP = 0x92,
	_CR3_WRITE_TRAP = 0x93,
	_CR4_WRITE_TRAP = 0x94,
	_CR5_WRITE_TRAP = 0x95,
	_CR6_WRITE_TRAP = 0x96,
	_CR7_WRITE_TRAP = 0x97,
	_CR8_WRITE_TRAP = 0x98,
	_CR9_WRITE_TRAP = 0x99,
	_CR10_WRITE_TRAP = 0x9a,
	_CR11_WRITE_TRAP = 0x9b,
	_CR12_WRITE_TRAP = 0x9c,
	_CR13_WRITE_TRAP = 0x9d,
	_CR14_WRITE_TRAP = 0x9e,
	_CR15_WRITE_TRAP = 0x9f,

	// VMEXIT_INVLPGB; INVLPGB instruction
	_INVLPGB = 0xA0,

	// VMEXIT_INVLPGB_ILLEGAL; Illegal INVLPGB instruction
	_INVLPGB_ILLEGAL = 0xA1,

	// VMEXIT_INVPCID; INVPCID instruction
	_INVPCID = 0xA2,

	// VMEXIT_MCOMMIT; MCOMMIT instruction
	_MCOMMIT = 0xA3,

	// VMEXIT_TLBSYNC; TLBSYNC instruction
	_TLBSYNC = 0xA4,

	// VMEXIT_NPF;
	// Nested paging : host - level page fault occurred(EXITINFO1
	//    contains fault error code; EXITINFO2 contains the guest
	//    physical address causing the fault.)
	_NPF = 0x400,

	// AVIC_INCOMPLETE_IPI; AVIC—Virtual IPI delivery not completed. See "AVIC IPI
	// Delivery Not Completed" for EXITINFO1–2 definitions.
	_INCOMPLETE_IPI = 0x401,

	// AVIC_NOACCEL; AVIC—Attemped access by guest to vAPIC register not
	// handled by AVIC hardware.See "AVIC Access to unaccelerated vAPIC register" for EXITINFO1–2 definitions.
	AVIC_NOACCEL = 0x402,

	// VMEXIT_VMGEXIT; VMGEXIT instruction
	_VMGEXIT = 0x403,

	// VMEXIT_INVALID; Invalid guest state in VMCB
	_INVALID = -1,

	// VMEXIT_BUSY; BUSY bit was set in the encrypted VMSA (see "Interrupt
	// Injection Restrictions")
	_BUSY = -2
};