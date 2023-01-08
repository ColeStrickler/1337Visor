#pragma once
#include <ntifs.h>
#include <intrin.h>
#include "cpu_codes.h"
struct IntrCr
{
    // INTERCEPT READS TO CR0-CR15
    WORD rIntrCR0 : 1;
    WORD rIntrCR1 : 1;
    WORD rIntrCR2 : 1;
    WORD rIntrCR3 : 1;
    WORD rIntrCR4 : 1;
    WORD rIntrCR5 : 1;
    WORD rIntrCR6 : 1;
    WORD rIntrCR7 : 1;
    WORD rIntrCR8 : 1;
    WORD rIntrCR9 : 1;
    WORD rIntrCR10 : 1;
    WORD rIntrCR11 : 1;
    WORD rIntrCR12 : 1;
    WORD rIntrCR13 : 1;
    WORD rIntrCR14 : 1;
    WORD rIntrCR15 : 1;

    // INTERCEPT WRITES TO CR0-CR15
    WORD wIntrCR0 : 1;
    WORD wIntrCR1 : 1;
    WORD wIntrCR2 : 1;
    WORD wIntrCR3 : 1;
    WORD wIntrCR4 : 1;
    WORD wIntrCR5 : 1;
    WORD wIntrCR6 : 1;
    WORD wIntrCR7 : 1;
    WORD wIntrCR8 : 1;
    WORD wIntrCR9 : 1;
    WORD wIntrCR10 : 1;
    WORD wIntrCR11 : 1;
    WORD wIntrCR12 : 1;
    WORD wIntrCR13 : 1;
    WORD wIntrCR14 : 1;
    WORD wIntrCR15 : 1;
};
static_assert(sizeof(IntrCr) == 0x4, "IntrCr size mismatch.");

struct IntrDr
{
    // INTERCEPT READS TO DR0-DR15
    WORD rIntrDR0 : 1;
    WORD rIntrDR1 : 1;
    WORD rIntrDR2 : 1;
    WORD rIntrDR3 : 1;
    WORD rIntrDR4 : 1;
    WORD rIntrDR5 : 1;
    WORD rIntrDR6 : 1;
    WORD rIntrDR7 : 1;
    WORD rIntrDR8 : 1;
    WORD rIntrDR9 : 1;
    WORD rIntrDR10 : 1;
    WORD rIntrDR11 : 1;
    WORD rIntrDR12 : 1;
    WORD rIntrDR13 : 1;
    WORD rIntrDR14 : 1;
    WORD rIntrDR15 : 1;

    // INTERCEPT WRITES TO DR0-DR15
    WORD wIntrDR0 : 1;
    WORD wIntrDR1 : 1;
    WORD wIntrDR2 : 1;
    WORD wIntrDR3 : 1;
    WORD wIntrDR4 : 1;
    WORD wIntrDR5 : 1;
    WORD wIntrDR6 : 1;
    WORD wIntrDR7 : 1;
    WORD wIntrDR8 : 1;
    WORD wIntrDR9 : 1;
    WORD wIntrDR10 : 1;
    WORD wIntrDR11 : 1;
    WORD wIntrDR12 : 1;
    WORD wIntrDR13 : 1;
    WORD wIntrDR14 : 1;
    WORD wIntrDR15 : 1;
};
static_assert(sizeof(IntrDr) == 0x4, "IntrDr size mismatch.");


struct IntrMisc1
{
    DWORD IntrPhys : 1;                         // 0. Intercept physical maskable interrupt
    DWORD IntrNMI : 1;                          // 1. Intercept NMI
    DWORD IntrSMI : 1;                          // 2. Intercept SMI
    DWORD IntrINIT : 1;                         // 3. Intercept INIT
    DWORD IntrVINTR : 1;                        // 4. Intercept virtual maskable interrupt
    DWORD wIntrCR0 : 1;                         // 5. Intercept CR0 writes to CR0.TS or CR0.MP
    DWORD rIntrIDTR : 1;                        // 6. Intercept reads of IDTR
    DWORD rIntrGDTR : 1;                        // 7. Intercept reads of GDTR
    DWORD rIntrLDTR : 1;                        // 8. Intercept reads of LDTR
    DWORD rIntrTR : 1;                          // 9. Intercept reads of TR
    DWORD wIntrIDTR : 1;                        // 10. Intercept writes of IDTR
    DWORD wIntrGDTR : 1;                        // 11. Intercept writes of GDTR
    DWORD wIntrLDTR : 1;                        // 12. Intercept writes of LDTR
    DWORD wIntrTr : 1;                          // 13. Intercept writes of Tr
    DWORD IntrRDTSC : 1;                        // 14. Intercept RDTSC instruction
    DWORD IntrRDPMC : 1;                        // 15. Intercept RDPMC instruction
    DWORD IntrPUSHF : 1;                        // 16. Intercept PUSHF instruction
    DWORD IntrPOPF : 1;                         // 17. Intercept POPF instruction
    DWORD IntrCPUID : 1;                        // 18. Intercept CPUID intstruction
    DWORD IntrRSM : 1;                          // 19. Intercept RSM instruction
    DWORD IntrIRET : 1;                         // 20. Intercept IRET instruction
    DWORD IntrINTn : 1;                         // 21. Intercept INTn instructions
    DWORD IntrINVD : 1;                         // 22. Intercept INVD instruction
    DWORD IntrPAUSE : 1;                        // 23. Intercept PAUSE instruction
    DWORD IntrHLT : 1;                          // 24. Intercept HLT instruction
    DWORD IntrINVLPG : 1;                       // 25. Intercept INVLPG instruction
    DWORD IntrINVLPGA : 1;                      // 26. Intercept INVLPGA instruction
    DWORD IntrIO : 1;                           // 27. Intercept IO to selected ports
    DWORD IntrMSR : 1;                          // 28. Intercept wrmsr & rdmsr to selected MSR's
    DWORD IntrTS : 1;                           // 29. Intercept task switches
    DWORD IntrFreeze : 1;                       // 30. Intercept FERR_FREEZE processor freezing during legacy FERR handling
    DWORD IntrShutDwn : 1;                      // 31. Intercept shutdown events
};
static_assert(sizeof(IntrMisc1) == 0x4, "IntrMisc1 size mismatch.");


struct IntrMisc2
{
    DWORD IntrVMRUN : 1;
    DWORD IntrVMCALL : 1;
    DWORD IntrVMLOAD : 1;
    DWORD IntrVMSAVE : 1;
    DWORD IntrSTGI : 1;
    DWORD IntrCLGI : 1;
    DWORD IntrSKINIT : 1;
    DWORD IntrRDTSCP : 1;
    DWORD IntrICEBP : 1;
    DWORD IntrWB : 1;
    DWORD IntrMONITOR : 1;
    DWORD IntrMWAIT : 1;
    DWORD IntrMWAIT2 : 1;
    DWORD IntrXSETBV : 1;
    DWORD IntrRDPRU : 1;
    DWORD IntrEFER : 1;
    DWORD IntrCRx : 16;
};

static_assert(sizeof(IntrMisc2) == 0x4, "IntrMisc2 size mismatch.");

struct ExitIntInfo
{
    uintptr_t Vector :                  8;
    uintptr_t Type :                    3;
    uintptr_t ErrorCodeValid:           1;
    uintptr_t Reserverd :               19;
    uintptr_t Valid :                   1;
    uintptr_t ErrorCode :               32;
};
static_assert(sizeof(ExitIntInfo) == 0x8, "ExitIntInfo size mismatch.");

struct NestedPagingEnable
{
    uintptr_t NP_ENABLE :                       1;
    uintptr_t SecureEncryptedVirtualization :   1;
    uintptr_t EncryptedStateEnable :            1;
    uintptr_t GuestModeExecuteTrap :            1;
    uintptr_t SSSCheckEn :                      1;
    uintptr_t VirtualTransparentEncryption :    1;
    uintptr_t ReadOnlyGuestPT :                 1;
    uintptr_t EnableInstr :                     1;
    uintptr_t Reserved :                        56;
};
static_assert(sizeof(NestedPagingEnable) == 0x8, "NpEnable size mismatch.");


struct CleanField
{
    uintptr_t Intercepts :  1;                   // 0. All the intercept vectors, TSC Offset, Pause Filter Count
    uintptr_t IOPM :        1;                   // 1. IOMSRPM: IOPM_BASE, MSRPM_BASE
    uintptr_t ASID :        1;                   // 2. ASID
    uintptr_t TPR :         1;                   // 3. V_TPR, V_IRQ, V_INTR, V_INTR_PRIO, V_IGN_TPR, V_INTR_MASKING, V_INTR_VECTOR
    uintptr_t NP :          1;                   // 4. Nested Paging: NCR3, G_PAT
    uintptr_t CRx :         1;                   // 5. CR0, CR3, CR4, EFER
    uintptr_t DRx :         1;                   // 6. DR6, DR7
    uintptr_t DT :          1;                   // 7. GDT/IDT Limit and base
    uintptr_t SEG :         1;                   // 8. CS/DS/SS/ES Sel/Base/Limit/Attr, CPL
    uintptr_t CR2 :         1;                   // 9. Cr2
    uintptr_t LBR :         1;                   // 10. LBR DbgCtlMsr, br_from/to, lastint_from/to
    uintptr_t AVIC :        1;                   // 11. AVIC APIC_BAR, AVIC APIC_BACKING_PAGE, AVIC PHYSICAL_TABLE and AVIC LOGICAL_TABLE pointers
    uintptr_t CET :         1;                   // 12. S_CET, SSP, ISST_ADDR
    uintptr_t Reserved :    19;
    uintptr_t Reserved2 :   32;
};
static_assert(sizeof(CleanField) == 0x8, "CleanField size mismatch.");

struct EventInj
{
    uintptr_t Vector : 8;
    uintptr_t Type : 3;
    uintptr_t ErrorCodeValid : 1;
    uintptr_t Reserved : 19;
    uintptr_t Valid : 1;
    uintptr_t ErrorCode : 32;
};
static_assert(sizeof(EventInj) == 0x8, "EventINj size mismatch.");




typedef struct _CONTROL_AREA
{
    IntrCr InterceptCr;                     // +0x000
    IntrDr InterceptDr;                     // +0x004
    DWORD InterceptExceptionVec;            // +0x008
    IntrMisc1 InterceptMisc1;               // +0x00c
    IntrMisc2 InterceptMisc2;               // +0x010
    BYTE Reserved1[0x03c - 0x014];          // +0x014
    WORD PauseFilterThreshold;              // +0x03c
    WORD PauseFilterCount;                  // +0x03e
    uintptr_t IOPM_BASE_PA;                 // +0x040
    uintptr_t MSRPM_BASE_PA;                // +0x048
    uintptr_t TscOffset;                    // +0x050
    DWORD GuestAsid;                        // +0x058
    DWORD TlbControl : 8;                   // +0x05c
    DWORD Reserved_SBZ : 24;                // +0x05d
    uintptr_t VirtualIntr;                  // +0x060
    uintptr_t InterruptShadow;              // +0x068
    uintptr_t ExitCode;                     // +0x070
    uintptr_t ExitInfo1;                    // +0x078
    uintptr_t ExitInfo2;                    // +0x080
    ExitIntInfo ExitIntInfo;                // +0x088
    NestedPagingEnable NpEnable;            // +0x090
    uintptr_t AvicApicBar;                  // +0x098
    uintptr_t GuestPaOfGhcb;                // +0x0a0
    EventInj EventInj;                     // +0x0a8
    uintptr_t NCr3;                         // +0x0b0
    uintptr_t LbrVirtualizationEnable;      // +0x0b8
    CleanField CleanBits;                   // +0x0c0
    uintptr_t NRip;                         // +0x0c8
    BYTE NumOfBytesFetched;                 // +0x0d0
    BYTE GuestInstructionBytes[15];         // +0x0d1
    uintptr_t AvicApicBackingPagePointer;   // +0x0e0
    uintptr_t Reserved2;                    // +0x0e8
    uintptr_t AvicLogicalTablePointer;      // +0x0f0
    uintptr_t AvicPhysicalTablePointer;     // +0x0f8
    uintptr_t Reserved3;                    // +0x100
    uintptr_t VmcbSaveStatePointer;         // +0x108
    BYTE Reserved4[0x400 - 0x110];          // +0x110
} CONTROL_AREA, * PCONTROL_AREA;
static_assert(sizeof(_CONTROL_AREA) == 0x400, "CONTROL_AREA Size Mismatch");




struct seg_register
{

    WORD selector;
    WORD attribute;
    DWORD limit;
    uintptr_t base_addr;
};






typedef struct _STATE_SAVE_AREA
{
    
    seg_register es;                        // +0x000
    seg_register cs;                        // +0x010
    seg_register ss;                        // +0x020
    seg_register ds;                        // +0x030
    seg_register fs;                        // +0x040
    seg_register gs;                        // +0x050
    seg_register gdtr;                      // +0x060
    seg_register ldtr;                      // +0x070
    seg_register idtr;                      // +0x080
    seg_register tr;                        // +0x090

    BYTE Reserved1[0x0cb - 0x0a0];          // +0x0a0
    BYTE Cpl;                               // +0x0cb
    DWORD Reserved2;                        // +0x0cc
    uintptr_t Efer;                         // +0x0d0
    BYTE Reserved3[0x148 - 0x0d8];          // +0x0d8
    uintptr_t Cr4;                          // +0x148
    uintptr_t Cr3;                          // +0x150
    uintptr_t Cr0;                          // +0x158
    uintptr_t Dr7;                          // +0x160
    uintptr_t Dr6;                          // +0x168
    uintptr_t Rflags;                       // +0x170
    uintptr_t Rip;                          // +0x178
    BYTE Reserved4[0x1d8 - 0x180];          // +0x180
    uintptr_t Rsp;                          // +0x1d8
    BYTE Reserved5[0x1f8 - 0x1e0];          // +0x1e0
    uintptr_t Rax;                          // +0x1f8
    uintptr_t Star;                         // +0x200
    uintptr_t LStar;                        // +0x208
    uintptr_t CStar;                        // +0x210
    uintptr_t SfMask;                       // +0x218
    uintptr_t KernelGsBase;                 // +0x220
    uintptr_t SysenterCs;                   // +0x228
    uintptr_t SysenterEsp;                  // +0x230
    uintptr_t SysenterEip;                  // +0x238
    uintptr_t Cr2;                          // +0x240
    BYTE Reserved6[0x268 - 0x248];          // +0x248
    uintptr_t GPat;                         // +0x268
    uintptr_t DbgCtl;                       // +0x270
    uintptr_t BrFrom;                       // +0x278
    uintptr_t BrTo;                         // +0x280
    uintptr_t LastExcepFrom;                // +0x288
    uintptr_t LastExcepTo;                  // +0x290
} STATE_SAVE_AREA, * PSTATE_SAVE_AREA;
static_assert(sizeof(STATE_SAVE_AREA) == 0x298, "VMCB_STATE_SAVE_AREA Size Mismatch");



typedef struct _VMCB {
    CONTROL_AREA    ControlArea;
    STATE_SAVE_AREA StateSaveArea;
    BYTE Reserved[0x1000 - sizeof(CONTROL_AREA) - sizeof(STATE_SAVE_AREA)];
}VMCB, *PVMCB;
static_assert(sizeof(VMCB) == 0x1000, "VMCB Size Mismatch");
