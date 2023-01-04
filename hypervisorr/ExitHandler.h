#pragma once
#include "CpuStructs.h"
#include "InitVirt.h"


namespace exit_handler
{
	bool exitcheck(PVirtual_Processor_Data VprocData, PRegister_Context RegCtx);
	bool cpuid_handler(PVirtual_Processor_Data VprocData, PRegister_Context RegCtx);
	bool msr_access_handler(PVirtual_Processor_Data VprocData, PRegister_Context RegCtx);
	bool vmrun_handler(PVirtual_Processor_Data VprocData, PRegister_Context RegCtx);

};

