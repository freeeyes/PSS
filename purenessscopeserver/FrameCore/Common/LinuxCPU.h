#ifndef _LINUXCPU_H
#define _LINUXCPU_H

#include "define.h"
#include <fstream>
#include <string>
#include <sstream>
#include <fstream>

vector<string> split_string(const string& s, const char& c);

void trim(string& s);

int32 GetProcessCPU_Idel_Linux();

int32 GetProcessMemorySize_Linux();

string GetFileContent(string strFileName);

#endif
