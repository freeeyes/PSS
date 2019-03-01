#ifndef _WRITE_PLUGIN_MAKE_H
#define _WRITE_PLUGIN_MAKE_H

#include "XmlOpeation.h"

void Gen_2_Make_define(const char* pPath)
{
    char szTemp[MAX_CODE_LINE_SIZE]     = {'\0'};
    char szPathFile[MAX_CODE_LINE_SIZE] = {'\0'};

    sprintf_safe(szPathFile, MAX_CODE_LINE_SIZE, "%s/Makefile.define",
                 pPath);

    //首先生成声明文件。
    FILE* pFile = fopen(szPathFile, "w");

    if(NULL == pFile)
    {
        return;
    }

    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "# *****************************\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "# predefine\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "# *****************************\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "CC = g++\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "AR = ar\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "CFLAGS = -g -O2 -D__LINUX__\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "INCLUDES = -I./ -I../ -I/usr/include -I${ACE_ROOT} -I../include\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "LIBS = -L/usr/lib64 -L${ACE_ROOT}/lib  -L/usr/local/lib64 -L./ -L../ -lACE -lz -ldl\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "# *****************************\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "# change rule\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "# *****************************\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "# Here are some rules for converting .cpp -> .o\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, ".SUFFIXES: .cpp .o\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, ".cpp.o:\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "\t@$(CC) -fPIC $(CFLAGS) ${INCLUDES} -c $*.cpp\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "\t@echo '----- '$*.cpp' is compiled ok!'\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "# Here are some rules for converting .c -> .o\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, ".SUFFIXES: .c .o\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, ".c.o:\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "\t@$(CC) -fPIC $(CFLAGS)  ${INCLUDES} -c $*.c\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "\t@echo '----- '$*.c' is compiled ok!'\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    fclose(pFile);
}

void Gen_2_Make(const char* pPath, _Project_Info& objProjectInfo)
{
    char szTemp[MAX_CODE_LINE_SIZE]     = {'\0'};
    char szPathFile[MAX_CODE_LINE_SIZE] = {'\0'};

    sprintf_safe(szPathFile, MAX_CODE_LINE_SIZE, "%s/Makefile",
                 pPath);

    //首先生成声明文件。
    FILE* pFile = fopen(szPathFile, "w");

    if(NULL == pFile)
    {
        return;
    }

    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "include Makefile.define\n\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "default:all\n\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "default:all\n\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "PATS = BaseCommand.o %s.o\n\n",
                 objProjectInfo.m_szProjectName);
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "LIB_BASE_OBJS = BaseCommand.o %s.o %s_Logic.o\n\n",
                 objProjectInfo.m_szProjectName,
                 objProjectInfo.m_szProjectName);
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "LIB_BASE = lib%s.so\n\n",
                 objProjectInfo.m_szProjectName);
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "all:$(LIB_BASE) Makefile\n\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "$(LIB_BASE):$(PATS)\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "\t$(CC) -shared -o $(LIB_BASE) $(LIB_BASE_OBJS) $(LIBS)\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "clean:\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "\trm -rf *.o  $(LIB_BASE) $(LIB_BASE_OBJS)\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "clear:\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "\tmv lib%s.so ../../PurenessScopeServer/Linux_Bin/\n",
                 objProjectInfo.m_szProjectName);
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "cl:\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "\trm -rf *.o\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    fclose(pFile);
}

void Gen_2_RunLinuxMake(const char* pPath, _Project_Info& objProjectInfo)
{
    char szTemp[MAX_CODE_LINE_SIZE]     = {'\0'};
    char szPathFile[MAX_CODE_LINE_SIZE] = {'\0'};

    sprintf_safe(szPathFile, MAX_CODE_LINE_SIZE, "%s/runlinuxmake.sh",
                 pPath,
                 pPath);

    //首先生成声明文件。
    FILE* pFile = fopen(szPathFile, "wb");

    if(NULL == pFile)
    {
        return;
    }

    //set ff=unix
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "#!/bin/sh\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, ". ${HOME}/.bash_profile\n\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "mwc.pl -type gnuace\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

    fclose(pFile);
}

void Gen_2_Mpc(const char* pPath, _Project_Info& objProjectInfo)
{
    char szTemp[MAX_CODE_LINE_SIZE]     = {'\0'};
    char szPathFile[MAX_CODE_LINE_SIZE] = {'\0'};

    sprintf_safe(szPathFile, MAX_CODE_LINE_SIZE, "%s/%s.mpc",
                 pPath,
                 pPath);

    //首先生成声明文件。
    FILE* pFile = fopen(szPathFile, "w");

    if(NULL == pFile)
    {
        return;
    }

    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "project(%s):acelib{\n",
                 objProjectInfo.m_szProjectName);
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "\tsharedname=%s\n",
                 objProjectInfo.m_szProjectName);
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "\tdynamicflags += TEST_TCP_BUILD_DLL\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "\tlibout = ./\n\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "\tSource_Files{\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "\t\t./*.cpp\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "\t}\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "\tspecific (gnuace){\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "\t\tmacros += __LINUX__\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "\t\tcompile_flags += -Wno-deprecated\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "\t}\n\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "\tspecific (prop:microsoft){\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "\t\tdllout = ../../PurenessScopeServer/Windows_Bin\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "\t} else {\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "\t\tdllout = ../../PurenessScopeServer/Linux_Bin\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "\t}\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "\tincludes += .\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "\tincludes += ../../Common\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "}\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    fclose(pFile);
}

void Gen_2_ZZZ(const char* pPath)
{
    char szTemp[MAX_CODE_LINE_SIZE]     = {'\0'};
    char szPathFile[MAX_CODE_LINE_SIZE] = {'\0'};

    sprintf_safe(szPathFile, MAX_CODE_LINE_SIZE, "%s/zzz.bat",
                 pPath,
                 pPath);

    //首先生成声明文件。
    FILE* pFile = fopen(szPathFile, "w");

    if(NULL == pFile)
    {
        return;
    }

    sprintf_safe(szTemp, MAX_CODE_LINE_SIZE, "%%ACE_ROOT%%/bin/mwc.pl -type vc12\n");
    fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
    fclose(pFile);
}

#endif
