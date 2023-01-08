#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <intrin.h>


#define PRINT(x) std::cout << x << std::endl;
#define PRINTL(x, y) std::cout << x << ": " << y << std::endl


#define uint32 unsigned int
extern "C" bool DetectVmxSupport(void);


extern "C" uint32 call_cpuid(uint32 eax_in_val, uint32 ecx_in_val, uint32 * eax_out_ptr, uint32 * ebx_out_ptr, uint32 * ecx_out_ptr, uint32 * edx_out_ptr);

// CPUID CODES
#define CPUID_VENDOR_STRING						0X00000000
#define CPUID_PROCESSOR_FEATURE_IDENTIFIERS		0x80000001
#define CPUID_SVM_FEATURE_ID					0x8000000A


// FEATURE ID BIT SELECTORS
#define CPUID_FN8000_0001_ECX_SVM				(1 << 2)
#define CPUID_FN8000_000A_EDX_SVML				(1 << 2)



// MSR CODES
#define MSR_VM_CR								0xC0010114
#define MSR_VM_HSAVE_PA							0xC0010117		// Holds "host state-save" area, VMRUN will save host processor state info at this address



// MSR BIT SELECTORS
#define MSR_VM_CR_SVMDIS						(1 >> 4)


bool CheckCompatible() {

    // CpuInfo[0] = EAX, CpuInfo[1] = EBX, CpuInfo[2] = ECX, CpuInfo[3] = EDX
    int CpuInfo[4] = { 0 };

    __cpuid(CpuInfo, CPUID_VENDOR_STRING);
    if (
        (CpuInfo[1] != 'htuA') ||
        (CpuInfo[3] != 'itne') ||
        (CpuInfo[2] != 'DMAc'))
    {
        return FALSE;
    }
    memset(CpuInfo, 0, sizeof(CpuInfo));


    __cpuid(CpuInfo, CPUID_PROCESSOR_FEATURE_IDENTIFIERS);

    // Check if SVM is enabled
    if (!(CpuInfo[2] & CPUID_FN8000_0001_ECX_SVM)) {
        printf("No SVM support found.\n");
        return FALSE;
    }
    memset(CpuInfo, 0, sizeof(CpuInfo));


    __cpuid(CpuInfo, 0x8000000A);
    if (!(CpuInfo[3] & (1 << 0)))
    {
        printf("Fuckled\n");
        return FALSE;
    }






    // Read VM_CR MSR and see if the SVMDIS bit is set
    ULONG64 vm_cr_msr = __readmsr(MSR_VM_CR);
    if (!(vm_cr_msr & MSR_VM_CR_SVMDIS)) {
        printf("SVM support found.\n");
        return TRUE;
    }

    __cpuid(CpuInfo, CPUID_SVM_FEATURE_ID);
    // Check if Lock bit is set 
    if ((CpuInfo[4] & CPUID_FN8000_000A_EDX_SVML)) {
        printf("SVM disabled in BIOS.\n");
        return FALSE;
    }
    else {
        return FALSE;
    }

    return FALSE;
}




int main() {
    CheckCompatible();





	return -66;
}