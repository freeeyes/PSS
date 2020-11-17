#ifndef _WINDOWSDUMP_H
#define _WINDOWSDUMP_H

#include "define.h"

#if PSS_PLATFORM == PLATFORM_WIN

#include <windows.h>
#include <time.h>
#include <dbghelp.h>
#include <shellapi.h>
#include <tlhelp32.h>
#include <inttypes.h>
#include <string>
#include <algorithm>
#include <atlconv.h>

#define MAX_CRASH_REPORT_SIZE 150*1024

static inline const char*  getVersion()
{
    return "1.2.0";
}

typedef void(*crashhandler)(const char*, va_list, void*);

struct win_version_info
{
    int major;
    int minor;
    int build;
    int revis;
};
static size_t os_wcs_to_mbs(const wchar_t* str, size_t len, char* dst, size_t dst_size)
{
    size_t out_len;

    if (!str)
    {
        return 0;
    }

    out_len = dst ? (dst_size - 1) : wcstombs_s(&out_len, nullptr, 0, str, len);

    if (dst)
    {
        if (!dst_size)
        {
            return 0;
        }

        if (out_len)
        {
            wcstombs_s(&out_len, dst, dst_size, str, (unsigned long long)out_len + 1);
        }

        dst[out_len] = 0;
    }

    return out_len;
}

typedef DWORD(WINAPI* get_file_version_info_size_w_t)(
    LPCWSTR module,
    LPDWORD unused);
typedef BOOL(WINAPI* get_file_version_info_w_t)(
    LPCWSTR module,
    DWORD unused,
    DWORD len,
    LPVOID data);
typedef BOOL(WINAPI* ver_query_value_w_t)(
    LPVOID data,
    LPCWSTR subblock,
    LPVOID* buf,
    PUINT sizeout);

static get_file_version_info_size_w_t get_file_version_info_size = nullptr;
static get_file_version_info_w_t get_file_version_info = nullptr;
static ver_query_value_w_t ver_query_value = nullptr;
static bool ver_initialized = false;
static bool ver_initialize_success = false;
static bool initialize_version_functions(void)
{
    HMODULE ver = GetModuleHandleW(L"version");

    ver_initialized = true;

    if (!ver)
    {
        ver = LoadLibraryW(L"version");

        if (!ver)
        {
            OutputDebugStringW(L"Failed to load windows version library\n");
            return false;
        }
    }

    get_file_version_info_size = (get_file_version_info_size_w_t)
                                 GetProcAddress(ver, "GetFileVersionInfoSizeW");
    get_file_version_info = (get_file_version_info_w_t)
                            GetProcAddress(ver, "GetFileVersionInfoW");
    ver_query_value = (ver_query_value_w_t)
                      GetProcAddress(ver, "VerQueryValueW");

    if (!get_file_version_info_size ||
        !get_file_version_info ||
        !ver_query_value)
    {
        OutputDebugStringW(L"Failed to load windows version functions\n");
        return false;
    }

    ver_initialize_success = true;
    return true;
}

static bool get_dll_ver(const wchar_t* lib, struct win_version_info* ver_info)
{
    VS_FIXEDFILEINFO* info = nullptr;
    UINT len = 0;
    BOOL success;
    LPVOID data;
    DWORD size;

    if (!ver_initialized && !initialize_version_functions())
    {
        return false;
    }

    if (!ver_initialize_success)
    {
        return false;
    }


    size = get_file_version_info_size(lib, nullptr);

    if (!size)
    {
        OutputDebugStringW(L"Failed to get %s version info size\n");
        return false;
    }

    data = malloc(size);

    if (!get_file_version_info(lib, 0, size, data))
    {
        OutputDebugStringW(L"Failed to get %s version info\n");
        free(data);
        return false;
    }

    success = ver_query_value(data, L"\\", (LPVOID*)&info, &len);

    if (!success || !info || !len)
    {
        OutputDebugStringW(L"Failed to get %s version info value\n");
        free(data);
        return false;
    }

    ver_info->major = (int)HIWORD(info->dwFileVersionMS);
    ver_info->minor = (int)LOWORD(info->dwFileVersionMS);
    ver_info->build = (int)HIWORD(info->dwFileVersionLS);
    ver_info->revis = (int)LOWORD(info->dwFileVersionLS);

    free(data);
    return true;
}
#define WINVER_REG_KEY L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"

static void get_win_ver(struct win_version_info* info)
{
    static struct win_version_info ver = { 0 };
    static bool got_version = false;

    if (!info)
    {
        return;
    }

    if (!got_version)
    {
        get_dll_ver(L"kernel32", &ver);
        got_version = true;

        if (ver.major == 10)
        {
            HKEY    key;
            DWORD   size, win10_revision;
            LSTATUS status;

            status = RegOpenKeyW(HKEY_LOCAL_MACHINE,
                                 WINVER_REG_KEY, &key);

            if (status != ERROR_SUCCESS)
            {
                return;
            }

            size = sizeof(win10_revision);

            status = RegQueryValueExW(key, L"UBR", nullptr, nullptr,
                                      (LPBYTE)&win10_revision, &size);

            if (status == ERROR_SUCCESS)
                ver.revis = (int)win10_revision > ver.revis ?
                            (int)win10_revision : ver.revis;

            RegCloseKey(key);
        }
    }

    *info = ver;
}

typedef BOOL(WINAPI* ENUMERATELOADEDMODULES64)(HANDLE process,
        PENUMLOADED_MODULES_CALLBACK64 enum_loaded_modules_callback,
        PVOID user_context);
typedef DWORD(WINAPI* SYMSETOPTIONS)(DWORD sym_options);
typedef BOOL(WINAPI* SYMINITIALIZE)(HANDLE process, PCTSTR user_search_path,
                                    BOOL invade_process);
typedef BOOL(WINAPI* SYMCLEANUP)(HANDLE process);
typedef BOOL(WINAPI* STACKWALK64)(DWORD machine_type, HANDLE process,
                                  HANDLE thread, LPSTACKFRAME64 stack_frame,
                                  PVOID context_record,
                                  PREAD_PROCESS_MEMORY_ROUTINE64 read_memory_routine,
                                  PFUNCTION_TABLE_ACCESS_ROUTINE64 function_table_access_routine,
                                  PGET_MODULE_BASE_ROUTINE64 get_module_base_routine,
                                  PTRANSLATE_ADDRESS_ROUTINE64 translate_address);
typedef BOOL(WINAPI* SYMREFRESHMODULELIST)(HANDLE process);

typedef PVOID(WINAPI* SYMFUNCTIONTABLEACCESS64)(HANDLE process,
        DWORD64 addr_base);
typedef DWORD64(WINAPI* SYMGETMODULEBASE64)(HANDLE process, DWORD64 addr);
typedef BOOL(WINAPI* SYMFROMADDR)(HANDLE process, DWORD64 address,
                                  PDWORD64 displacement, PSYMBOL_INFOW symbol);
typedef BOOL(WINAPI* SYMGETLINEFROMADDRW64)(_In_ HANDLE hProcess,
        _In_ DWORD64 dwAddr,
        _Out_ PDWORD pdwDisplacement,
        _Out_ PIMAGEHLP_LINEW64 Line
                                           );
typedef BOOL(WINAPI* SYMGETMODULEINFO64)(HANDLE process, DWORD64 addr,
        PIMAGEHLP_MODULE64 module_info);

typedef DWORD64(WINAPI* SYMLOADMODULE64)(HANDLE process, HANDLE file,
        PSTR image_name, PSTR module_name, DWORD64 base_of_dll,
        DWORD size_of_dll);

typedef BOOL(WINAPI* MINIDUMPWRITEDUMP)(HANDLE process, DWORD process_id,
                                        HANDLE file, MINIDUMP_TYPE dump_type,
                                        PMINIDUMP_EXCEPTION_INFORMATION exception_param,
                                        PMINIDUMP_USER_STREAM_INFORMATION user_stream_param,
                                        PMINIDUMP_CALLBACK_INFORMATION callback_param);

typedef HINSTANCE(WINAPI* SHELLEXECUTEA)(HWND hwnd, LPCTSTR operation,
        LPCTSTR file, LPCTSTR parameters, LPCTSTR directory,
        INT show_flags);

struct stack_trace
{
    CONTEXT                               context;
    DWORD64                               instruction_ptr;
    STACKFRAME64                          frame;
    DWORD                                 image_type;
};

struct exception_handler_data
{
    SYMINITIALIZE                         sym_initialize;
    SYMCLEANUP                            sym_cleanup;
    SYMSETOPTIONS                         sym_set_options;
    SYMFUNCTIONTABLEACCESS64              sym_function_table_access64;
    SYMGETMODULEBASE64                    sym_get_module_base64;
    SYMFROMADDR                           sym_from_addr;
    SYMGETLINEFROMADDRW64                 sym_getline_frome_addr;
    SYMGETMODULEINFO64                    sym_get_module_info64;
    SYMREFRESHMODULELIST                  sym_refresh_module_list;
    STACKWALK64                           stack_walk64;
    ENUMERATELOADEDMODULES64              enumerate_loaded_modules64;
    MINIDUMPWRITEDUMP                     minidump_write_dump;

    HMODULE                               dbghelp;
    SYMBOL_INFOW*                          sym_info;
    PEXCEPTION_POINTERS                   exception;
    struct win_version_info               win_version;
    SYSTEMTIME                            time_info;
    HANDLE                                process;

    struct stack_trace                    main_trace;

    std::string                           str;
    std::string                         cpu_info;
    std::string                         module_name;
    std::string                         module_list;
};
static inline void exception_handler_data_free(
    struct exception_handler_data* data)
{
    LocalFree(data->sym_info);
    FreeLibrary(data->dbghelp);
}
static inline void* get_proc(HMODULE module, const char* func)
{
    return (void*)GetProcAddress(module, func);
}

#define GET_DBGHELP_IMPORT(type,target, str) \
    do { \
        data->target = (type)get_proc(data->dbghelp, str); \
        if (!data->target) \
            return false; \
    } while (false)

static inline bool get_dbghelp_imports(struct exception_handler_data* data)
{
    data->dbghelp = LoadLibraryW(L"DbgHelp");

    if (!data->dbghelp)
    {
        return false;
    }

    GET_DBGHELP_IMPORT(SYMINITIALIZE, sym_initialize, "SymInitialize");
    GET_DBGHELP_IMPORT(SYMCLEANUP, sym_cleanup, "SymCleanup");
    GET_DBGHELP_IMPORT(SYMSETOPTIONS, sym_set_options, "SymSetOptions");
    GET_DBGHELP_IMPORT(SYMFUNCTIONTABLEACCESS64, sym_function_table_access64,
                       "SymFunctionTableAccess64");
    GET_DBGHELP_IMPORT(SYMGETMODULEBASE64, sym_get_module_base64, "SymGetModuleBase64");
    GET_DBGHELP_IMPORT(SYMFROMADDR, sym_from_addr, "SymFromAddrW");
    GET_DBGHELP_IMPORT(SYMGETLINEFROMADDRW64, sym_getline_frome_addr, "SymGetLineFromAddrW64");

    GET_DBGHELP_IMPORT(SYMGETMODULEINFO64, sym_get_module_info64, "SymGetModuleInfo64");
    GET_DBGHELP_IMPORT(SYMREFRESHMODULELIST, sym_refresh_module_list, "SymRefreshModuleList");
    GET_DBGHELP_IMPORT(STACKWALK64, stack_walk64, "StackWalk64");
    GET_DBGHELP_IMPORT(ENUMERATELOADEDMODULES64, enumerate_loaded_modules64,
                       "EnumerateLoadedModulesW64");
    GET_DBGHELP_IMPORT(MINIDUMPWRITEDUMP, minidump_write_dump, "MiniDumpWriteDump");

    return true;
}
static inline void init_instruction_data(struct stack_trace* trace)
{
#ifdef _WIN64
    trace->instruction_ptr = trace->context.Rip;
    trace->frame.AddrPC.Offset = trace->instruction_ptr;
    trace->frame.AddrFrame.Offset = trace->context.Rbp;
    trace->frame.AddrStack.Offset = trace->context.Rsp;
    trace->image_type = IMAGE_FILE_MACHINE_AMD64;
#else
    trace->instruction_ptr = trace->context.Eip;
    trace->frame.AddrPC.Offset = trace->instruction_ptr;
    trace->frame.AddrFrame.Offset = trace->context.Ebp;
    trace->frame.AddrStack.Offset = trace->context.Esp;
    trace->image_type = IMAGE_FILE_MACHINE_I386;
#endif

    trace->frame.AddrFrame.Mode = AddrModeFlat;
    trace->frame.AddrPC.Mode = AddrModeFlat;
    trace->frame.AddrStack.Mode = AddrModeFlat;
}
static bool sym_initialize_called = false;

static inline void init_sym_info(struct exception_handler_data* data)
{
    data->sym_set_options(
        SYMOPT_UNDNAME |
        SYMOPT_FAIL_CRITICAL_ERRORS |
        SYMOPT_LOAD_ANYTHING);

    if (!sym_initialize_called)
    {
        data->sym_initialize(data->process, nullptr, true);
    }
    else
    {
        data->sym_refresh_module_list(data->process);
    }

    data->sym_info = (SYMBOL_INFOW*)LocalAlloc(LPTR, sizeof(*data->sym_info) + 256);
    data->sym_info->SizeOfStruct = sizeof(SYMBOL_INFOW);
    data->sym_info->MaxNameLen = 256;
}
static inline void init_version_info(struct exception_handler_data* data)
{
    get_win_ver(&data->win_version);
}

#define PROCESSOR_REG_KEY L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"
#define CPU_ERROR "<unable to query>"
static inline void init_cpu_info(struct exception_handler_data* data)
{
    HKEY key;
    LSTATUS status;

    status = RegOpenKeyW(HKEY_LOCAL_MACHINE, PROCESSOR_REG_KEY, &key);

    if (status == ERROR_SUCCESS)
    {
        wchar_t str[1024];
        DWORD size = 1024;

        status = RegQueryValueExW(key, L"ProcessorNameString", nullptr,
                                  nullptr, (LPBYTE)str, &size);

        if (status == ERROR_SUCCESS)
        {
            USES_CONVERSION;
            char* info = W2A(str);
            data->cpu_info = info;
        }
        else
        {
            data->cpu_info = CPU_ERROR;
        }
    }
    else
    {
        data->cpu_info = CPU_ERROR;
    }
}
static BOOL CALLBACK enum_all_modules(PCTSTR module_name, DWORD64 module_base,
                                      ULONG module_size, struct exception_handler_data* data)
{
    char name_ascii[MAX_PATH];
    USES_CONVERSION;
    char* module_name_ascii = (char* )module_name;
    strcpy_s(name_ascii, module_name_ascii);

    if (data->main_trace.instruction_ptr >= module_base &&
        data->main_trace.instruction_ptr < module_base + module_size)
    {
        data->module_name = name_ascii;
        transform(data->module_name.begin(), data->module_name.end(), data->module_name.begin(), ::tolower);
    }

    char szModeInfo[4096] = {};

#ifdef _WIN64
    sprintf_s(szModeInfo, 4096, "%016llX-%016llX %s\r\n", module_base, module_base + module_size, name_ascii);

#else
    sprintf_s(szModeInfo, 4096, "%08llX-%08llX %s\r\n", module_base, module_base + module_size, name_ascii);
#endif
    data->module_list += szModeInfo;
    return true;
}
static inline void init_module_info(struct exception_handler_data* data)
{
    data->enumerate_loaded_modules64(data->process,
                                     (PENUMLOADED_MODULES_CALLBACK64)enum_all_modules,
                                     data);
}
bool is_64_bit_windows(void)
{
#if defined(_WIN64)
    return true;
#elif defined(_WIN32)
    BOOL b64 = false;
    return IsWow64Process(GetCurrentProcess(), &b64) && b64;
#endif
}

static inline void write_header(struct exception_handler_data* data)
{
    char date_time[80];
    time_t now = time(0);
    struct tm ts;
    localtime_s(&ts, &now);
    strftime(date_time, sizeof(date_time), "%Y-%m-%d, %X", &ts);

    const char* obs_bitness;

    if (sizeof(void*) == 8)
    {
        obs_bitness = "64";
    }
    else
    {
        obs_bitness = "32";
    }


    const char* version = getVersion();
    char fullstr[40960];
    sprintf_s(fullstr, 40960, "Unhandled exception: %x\n"
              "Date/Time: %s\n"
              "Fault address: %llx (%s)\n"
              "PSS version: \"%s\" (%s-bit)\n"
              "Windows version: %d.%d build %d (revision: %d; %s-bit)\n"
              "CPU: %s\n", data->exception->ExceptionRecord->ExceptionCode,
              date_time,
              data->main_trace.instruction_ptr,
              data->module_name.c_str(),
              version,
              obs_bitness,
              data->win_version.major, data->win_version.minor,
              data->win_version.build, data->win_version.revis,
              is_64_bit_windows() ? "64" : "32",
              data->cpu_info.c_str());
    data->str += fullstr;

}
struct module_info
{
    DWORD64 addr;
    char name_ascii[MAX_PATH];
};
static BOOL CALLBACK enum_module(PCWSTR module_name, DWORD64 module_base,
                                 ULONG module_size, struct module_info* info)
{
    if (info->addr >= module_base &&
        info->addr < module_base + module_size)
    {


        USES_CONVERSION;
        char* name_ascii = W2A(module_name);
        strcpy_s(info->name_ascii, name_ascii);
        _strlwr_s(info->name_ascii);
        return false;
    }

    return true;
}
static inline void get_module_name(struct exception_handler_data* data,
                                   struct module_info* info)
{
    data->enumerate_loaded_modules64(data->process,
                                     (PENUMLOADED_MODULES_CALLBACK64)enum_module, info);
}

static inline bool walk_stack(struct exception_handler_data* data,
                              HANDLE thread, struct stack_trace* trace)
{
    struct module_info module_info = { 0 };
    DWORD64 func_offset;
    char sym_name[256];
    char* p;

    bool success = data->stack_walk64(trace->image_type,
                                      data->process, thread, &trace->frame, &trace->context,
                                      nullptr, data->sym_function_table_access64,
                                      data->sym_get_module_base64, nullptr);

    if (!success)
    {
        return false;
    }

    module_info.addr = trace->frame.AddrPC.Offset;
    get_module_name(data, &module_info);

    if (!!module_info.name_ascii[0])
    {
        p = strrchr(module_info.name_ascii, '\\');
        p = p ? (p + 1) : module_info.name_ascii;
    }
    else
    {
        strcpy_s(module_info.name_ascii, MAX_PATH, "<unknown>");
        p = module_info.name_ascii;
    }

    success = !!data->sym_from_addr(data->process,
                                    trace->frame.AddrPC.Offset, &func_offset,
                                    data->sym_info);


    bool hasline = false;
    // Attempt to retrieve line number information.
    DWORD line_displacement = 0;
    IMAGEHLP_LINEW64 line = {};
    line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
    hasline = data->sym_getline_frome_addr(data->process, trace->frame.AddrPC.Offset, &line_displacement, &line);

    if (success)
    {
        os_wcs_to_mbs(data->sym_info->Name, 0, sym_name, 256);

    }


#ifdef _WIN64
#define SUCCESS_FORMAT \
    "%016I64X %016I64X %016I64X %016I64X " \
    "%016I64X %016I64X %s!%s+0x%I64x %S %d\r\n"
#define FAIL_FORMAT \
    "%016I64X %016I64X %016I64X %016I64X " \
    "%016I64X %016I64X %s!0x%I64x\r\n"
#else
#define SUCCESS_FORMAT \
    "%08.8I64X %08.8I64X %08.8I64X %08.8I64X " \
    "%08.8I64X %08.8I64X %s!%s+0x%I64x %S %d\r\n"
#define FAIL_FORMAT \
    "%08.8I64X %08.8I64X %08.8I64X %08.8I64X " \
    "%08.8I64X %08.8I64X %s!0x%I64x\r\n"

    trace->frame.AddrStack.Offset &= 0xFFFFFFFFF;
    trace->frame.AddrPC.Offset &= 0xFFFFFFFFF;
    trace->frame.Params[0] &= 0xFFFFFFFF;
    trace->frame.Params[1] &= 0xFFFFFFFF;
    trace->frame.Params[2] &= 0xFFFFFFFF;
    trace->frame.Params[3] &= 0xFFFFFFFF;
#endif
    char formatstr[40960];

    if (success && (data->sym_info->Flags & SYMFLAG_EXPORT) == 0)
    {

        sprintf_s(formatstr, 40960, SUCCESS_FORMAT,
                  trace->frame.AddrStack.Offset,
                  trace->frame.AddrPC.Offset,
                  trace->frame.Params[0],
                  trace->frame.Params[1],
                  trace->frame.Params[2],
                  trace->frame.Params[3],
                  p, sym_name, func_offset, hasline ? line.FileName : L"no filename", hasline ? line.LineNumber : 0);
    }
    else
    {
        sprintf_s(formatstr, 40960, FAIL_FORMAT,
                  trace->frame.AddrStack.Offset,
                  trace->frame.AddrPC.Offset,
                  trace->frame.Params[0],
                  trace->frame.Params[1],
                  trace->frame.Params[2],
                  trace->frame.Params[3],
                  p, trace->frame.AddrPC.Offset);
    }

    data->str += formatstr;

    return true;
}

#ifdef _WIN64
#define TRACE_TOP \
    "Stack            EIP              Arg0             " \
    "Arg1             Arg2             Arg3             " \
    "Address		  filename		   line\r\n"
#else
#define TRACE_TOP \
    "Stack    EIP      Arg0     " \
    "Arg1     Arg2     Arg3     Address\r\n"
#endif

static inline void write_thread_trace(struct exception_handler_data* data,
                                      THREADENTRY32* entry, bool first_thread)
{
    bool crash_thread = entry->th32ThreadID == GetCurrentThreadId();
    struct stack_trace trace = { 0 };
    struct stack_trace* ptrace;
    HANDLE thread;

    if (first_thread != crash_thread)
    {
        return;
    }

    if (entry->th32OwnerProcessID != GetCurrentProcessId())
    {
        return;
    }

    thread = OpenThread(THREAD_ALL_ACCESS, false, entry->th32ThreadID);

    if (!thread)
    {
        return;
    }

    trace.context.ContextFlags = CONTEXT_ALL;
    GetThreadContext(thread, &trace.context);
    init_instruction_data(&trace);

    char formatstr[MAX_PATH];
    sprintf_s(formatstr, MAX_PATH, "\r\nThread %lX%s\r\n%s",
              entry->th32ThreadID,
              crash_thread ? " (Crashed)" : "", TRACE_TOP);

    data->str += formatstr;
    ptrace = crash_thread ? &data->main_trace : &trace;

    while (walk_stack(data, thread, ptrace));

    CloseHandle(thread);
}

static inline void write_thread_traces(struct exception_handler_data* data)
{
    THREADENTRY32 entry = { 0 };
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,
                      GetCurrentProcessId());
    bool success;

    if (snapshot == INVALID_HANDLE_VALUE)
    {
        return;
    }

    entry.dwSize = sizeof(entry);

    success = !!Thread32First(snapshot, &entry);

    while (success)
    {
        write_thread_trace(data, &entry, true);
        success = !!Thread32Next(snapshot, &entry);
    }

    success = !!Thread32First(snapshot, &entry);

    while (success)
    {
        write_thread_trace(data, &entry, false);
        success = !!Thread32Next(snapshot, &entry);
    }

    CloseHandle(snapshot);
}

static inline void write_module_list(struct exception_handler_data* data)
{
    data->str += "\r\nLoaded modules:\r\n";
#ifdef _WIN64
    data->str += "Base Address                      Module\r\n";
#else
    data->str += "Base Address      Module\r\n";
#endif
    data->str += data->module_list;
}

/* ------------------------------------------------------------------------- */

static inline void handle_exception(struct exception_handler_data* data,
                                    PEXCEPTION_POINTERS exception)
{
    if (!get_dbghelp_imports(data))
    {
        return;
    }

    data->exception = exception;
    data->process = GetCurrentProcess();
    data->main_trace.context = *exception->ContextRecord;
    GetSystemTime(&data->time_info);

    init_sym_info(data);
    init_version_info(data);
    init_cpu_info(data);
    init_instruction_data(&data->main_trace);
    init_module_info(data);

    write_header(data);
    write_thread_traces(data);
    write_module_list(data);
}


#ifdef _MSC_VER
#define NORETURN __declspec(noreturn)
#else
#define NORETURN __attribute__((noreturn))
#endif

NORETURN static void def_crash_handler(const char* format, va_list args,
                                       void* param)
{
    vfprintf(stderr, format, args);
    exit(0);

}

static int  crashing = 0;
static void* crash_param = nullptr;
static crashhandler _crashhandler = def_crash_handler;

void bcrash(const char* format, ...)
{
    va_list args;

    if (crashing)
    {
        fputs("Crashed in the crash handler", stderr);
        exit(2);
    }

    crashing = 1;
    va_start(args, format);
    _crashhandler(format, args, crash_param);
    va_end(args);
}

static LONG CALLBACK exception_handler(PEXCEPTION_POINTERS exception)
{
    struct exception_handler_data data = { 0 };
    static bool inside_handler = false;

    /* don't use if a debugger is present */
    if (IsDebuggerPresent())
    {
        return EXCEPTION_CONTINUE_SEARCH;
    }

    if (inside_handler)
    {
        return EXCEPTION_CONTINUE_SEARCH;
    }

    inside_handler = true;
    handle_exception(&data, exception);
    bcrash(data.str.c_str());
    exception_handler_data_free(&data);
    inside_handler = false;
    return EXCEPTION_CONTINUE_SEARCH;
}

static void main_crash_handler(const char* format, va_list args, void* param)
{
    char* text = new char[MAX_CRASH_REPORT_SIZE];

    vsnprintf(text, MAX_CRASH_REPORT_SIZE, format, args);

    FILE* fp = nullptr;
    fopen_s(&fp, "crashlog.log", "w");

    fwrite(text, 1, strlen(text) + 1, fp);
    fclose(fp);


    exit(-1);
}

void base_set_crash_handler(crashhandler handle, void* param)
{
    crash_param = param;
    _crashhandler = handle;
}

#endif

#endif
