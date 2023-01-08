// WINDOWS AND LIBRARY HEADERS
#include <ntifs.h>
#include <intrin.h>


// OUR DEFINED HEADERS
#include "helpers.h"



// GLOBALS
Globals g_Struct;






static CALLBACK_FUNCTION PowerCallback;
DRIVER_UNLOAD Unload;



static VOID PowerCallback(
    PVOID CallbackContext,
    PVOID Argument1,
    PVOID Argument2
)
{
    UNREFERENCED_PARAMETER(CallbackContext);

    //
    // PO_CB_SYSTEM_STATE_LOCK of Argument1 indicates that a system power state
    // change is imminent.
    //
    if (Argument1 != reinterpret_cast<PVOID>(PO_CB_SYSTEM_STATE_LOCK))
    {
        goto Exit;
    }

    if (Argument2 != FALSE)
    {
        //
        // The system has just reentered S0. Re-virtualize all processors.
        // The system has just reentered S0. Re-virtualize all processors.
        //
        NT_VERIFY(NT_SUCCESS(virt::InitVirtualization(&g_Struct)));
    }
    else
    {
        //
        // The system is about to exit system power state S0. De-virtualize all
        // processors.
        //
        virt::ExitVirtualSession();
    }

Exit:
    return;
}















NTSTATUS CompleteIrp(PIRP Irp, NTSTATUS status = STATUS_SUCCESS, ULONG_PTR info = 0) {
    Irp->IoStatus.Status = status;
    Irp->IoStatus.Information = info;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return status;
}

NTSTATUS DrvUnsupported(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    UNREFERENCED_PARAMETER(DeviceObject);
    KdPrint(("Function is not supported.\n"));
    return CompleteIrp(Irp);
}


NTSTATUS DrvWrite(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    UNREFERENCED_PARAMETER(DeviceObject);
    KdPrint(("Function is not supported.\n"));
    return CompleteIrp(Irp);
}



NTSTATUS DrvRead(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    UNREFERENCED_PARAMETER(DeviceObject);
    KdPrint(("Function is not supported.\n"));
    return CompleteIrp(Irp);
}


NTSTATUS DrvIoctlDispatcher(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    UNREFERENCED_PARAMETER(DeviceObject);
    KdPrint(("Function is not supported.\n"));
    return CompleteIrp(Irp);
}


NTSTATUS DrvCreate(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    UNREFERENCED_PARAMETER(DeviceObject);
    KdPrint(("Function is not supported.\n"));
    return CompleteIrp(Irp);
}



NTSTATUS DrvClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    UNREFERENCED_PARAMETER(DeviceObject);
    KdPrint(("Function is not supported.\n"));
    return CompleteIrp(Irp);
}





BOOL CheckCompatible() {
    
    // CpuInfo[0] = EAX, CpuInfo[1] = EBX, CpuInfo[2] = ECX, CpuInfo[3] = EDX
    int CpuInfo[4] = { 0 };
    
    __cpuid(CpuInfo, CPUID_VENDOR_STRING);
    if (
        (CpuInfo[1] != 'htuA') ||
        (CpuInfo[3] != 'itne') ||
        (CpuInfo[2] != 'DMAc') ) 
    {
        return FALSE;
    }
    memset(CpuInfo, 0, sizeof(CpuInfo));


    __cpuid(CpuInfo, CPUID_PROCESSOR_FEATURE_IDENTIFIERS);

    // Check if SVM is enabled
    if (!(CpuInfo[2] & CPUID_FN8000_0001_ECX_SVM)) {
        KdPrint(("No SVM support found.\n"));
        return FALSE;
    }
    memset(CpuInfo, 0, sizeof(CpuInfo));
    
    __cpuid(CpuInfo, CPUID_SVM_FEATURE_ID);
    if (!(CpuInfo[3] & CPUID_FN8000_000A_EDX_NP)) {
        KdPrint(("No Nested Paging support found.\n"));
        return FALSE;
    }

    // Read VM_CR MSR and see if the SVMDIS bit is set
    ULONG64 vm_cr_msr = __readmsr(MSR_VM_CR);
    if (!(vm_cr_msr & MSR_VM_CR_SVMDIS)) {
        KdPrint(("SVM support found.\n"));
        return TRUE;
    }

    
    // Check if Lock bit is set 
    if ((CpuInfo[3] & CPUID_FN8000_000A_EDX_SVML)) {
        KdPrint(("SVM disabled in BIOS.\n"));
        return FALSE;
    }
    else {
        return FALSE;
    }
    
}




void Unload(PDRIVER_OBJECT DriverObject)
{
    UNICODE_STRING DosDeviceName = RTL_CONSTANT_STRING(L"\\DosDevices\\hypervisor");

    if (g_Struct.PowerStateCallback != nullptr) {
        ExUnregisterCallback(g_Struct.PowerStateCallback);
    }
    virt::DevirtualizeProcessors();


    IoDeleteSymbolicLink(&DosDeviceName);
    IoDeleteDevice(DriverObject->DeviceObject);
}


extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
    UNREFERENCED_PARAMETER(RegistryPath);
    NTSTATUS                status =                         STATUS_SUCCESS;
    PDEVICE_OBJECT          DeviceObject =                   NULL;
    UNICODE_STRING          DriverName,                      DosDeviceName;
    PCALLBACK_OBJECT        PowerCallbackObject;
    UNICODE_STRING          PowerObjectName =                RTL_CONSTANT_STRING(L"\\Callback\\PowerState");
    OBJECT_ATTRIBUTES       PowerCallbackObjAttr =           RTL_CONSTANT_OBJECT_ATTRIBUTES(&PowerObjectName, OBJ_CASE_INSENSITIVE);


    // BOOL CHECKS
    bool deviceCreated =            false;
    bool symLinkCreated =           false;
    bool powerCallbackCreated =     false;



   KdPrint(("DriverEntry Called."));
    RtlInitUnicodeString(&DriverName, L"\\Device\\hypervisor");
    RtlInitUnicodeString(&DosDeviceName, L"\\DosDevices\\hypervisor");



    // WE WILL DO ALL THE SETUP INSIDE THIS DO-WHILE-FALSE LOOP
    do {

        // Perform checks to see if SVM is supported on current CPU
        if (!CheckCompatible()) {
            status = STATUS_NOT_SUPPORTED;
            break;
        }
        KdPrint(("Found Compatible.\n"));

        status = ExCreateCallback(&PowerCallbackObject, &PowerCallbackObjAttr, FALSE, TRUE);
        if (!NT_SUCCESS(status)) {
            KdPrint(("Could not create PowerState Callback0.\n"));
            status = STATUS_CANCELLED;
            break;
        }
        g_Struct.PowerStateCallback = ExRegisterCallback(PowerCallbackObject, PowerCallback, nullptr);
        if (g_Struct.PowerStateCallback == nullptr) {
            KdPrint(("Could not register PowerState Callback1.\n"));
            status = STATUS_CANCELLED;
            break;
        }

        powerCallbackCreated = true;




        status = IoCreateDevice(DriverObject, 0, &DriverName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &DeviceObject);
        if (!NT_SUCCESS(status)) {
            KdPrint(("Could not create Device object.\n"));
            break;
        }
        deviceCreated = true;

        status = status = IoCreateSymbolicLink(&DosDeviceName, &DriverName);
        if (!NT_SUCCESS(status)) {
            KdPrint(("Could not create symbolic link.\n"));
            break;
        }
        symLinkCreated = true;


        for (short int i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++) {
            DriverObject->MajorFunction[i] = DrvUnsupported;
        }


        DriverObject->MajorFunction[IRP_MJ_CLOSE] =             DrvClose;
        DriverObject->MajorFunction[IRP_MJ_CREATE] =            DrvCreate;
        DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] =    DrvIoctlDispatcher;
        DriverObject->MajorFunction[IRP_MJ_READ] =              DrvRead;
        DriverObject->MajorFunction[IRP_MJ_WRITE] =             DrvWrite;
        DriverObject->DriverUnload =                            Unload;
        



        status = virt::InitVirtualization(&g_Struct);

    } while (false);

    if (!NT_SUCCESS(status)) {

        if (powerCallbackCreated && g_Struct.PowerStateCallback != nullptr) {
            ExUnregisterCallback(g_Struct.PowerStateCallback);
        }

        if (symLinkCreated) {
            IoDeleteSymbolicLink(&DosDeviceName);
        }

        if (deviceCreated) {
            IoDeleteDevice(DriverObject->DeviceObject);
        }


    }

    return status;
}



