#ifndef _LINUXCPU_H
#define _LINUXCPU_H

#include "define.h"

const char szVmRSS[] = "VmRSS:";
const char szVmSize[] = "VmSize:";

int32 GetProcessCPU_Idel_Linux();

int32 GetProcessMemorySize_Linux();

#endif
