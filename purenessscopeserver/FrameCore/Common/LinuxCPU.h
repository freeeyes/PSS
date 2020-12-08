#ifndef _LINUXCPU_H
#define _LINUXCPU_H

#include "define.h"
#include <fstream>
#include <string>
#include <sstream>
#include <fstream>

void trim(string& s);

int32 GetProcessCPU_Idel_Linux();

int32 GetProcessMemorySize_Linux();

string GetFileContent(const string& strFileName);

#endif
