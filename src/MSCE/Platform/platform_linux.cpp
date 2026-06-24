#include "platform.h"

#include <array>
#include <cstdlib>
#include <cxxabi.h>
#include <limits.h>
#include <unistd.h>
#include <dlfcn.h>
#include <elfutils/libdwfl.h>

namespace
{
    const msce::Logger logger = msce::Logger("Platform(Linux)");
    const Dwfl_Callbacks process_callbacks = {
        .find_elf = dwfl_linux_proc_find_elf,
        .find_debuginfo = dwfl_standard_find_debuginfo,
        .section_address = dwfl_offline_section_address,
        .debuginfo_path = nullptr};
}

std::string msce::Platform::demangle(const std::string &mangled, bool log) noexcept
{
    int s = 0;
    char *demangled = abi::__cxa_demangle(mangled.c_str(), nullptr, nullptr, &s);

    if (!demangled)
    {
        switch (s)
        {
        case -1:
            if (log)
                logger.log_error("Failed to demangle '{}'. Memory allocation failiure occured.", mangled);
            break;

        case -2:
            if (log)
                logger.log_error("Failed to demangle '{}'. Invalid mangled input.", mangled);
            break;

        case -3:
            if (log)
                logger.log_error("Failed to demangle '{}'. Invalid argument passed.", mangled);
            break;

        default:
            if (log)
                logger.log_error("Failed to demangle '{}'. Unknown issue.", mangled);
            break;
        }

        return mangled;
    }

    std::string res(demangled);
    free(demangled);
    return res;
}

std::string msce::Platform::get_absolute_executable_path()
{
    static const std::string cached = []()
    {
        char buffer[PATH_MAX];
        ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);

        if (len == -1)
        {
            logger.log_error("Failed to get current executable path from system. Returning empty string");
            return std::string("");
        }

        buffer[len] = '\0';
        std::string path(buffer);

        return path;
    }();

    return cached;
}

msce::Platform::SourceInfo msce::Platform::resolve_shared_object_address_source(void *address)
{
    SourceInfo res;
    Dl_info info;
    int s = dladdr(address, &info);
    if (s == 0)
    {
        logger.log_error("Failed to resolve shared object address {} in any linked libraries.", address);
        return res;
    }

    Dwfl *dwfl = dwfl_begin(&process_callbacks);
    if (!dwfl)
        return res;

    if (dwfl_linux_proc_report(dwfl, getpid()) != 0)
    {
        dwfl_end(dwfl);
        return res;
    }
    dwfl_report_end(dwfl, nullptr, nullptr);

    uintptr_t ip = reinterpret_cast<uintptr_t>(address);
    Dwfl_Module *module = dwfl_addrmodule(dwfl, ip);

    if (!module)
        return res;
    Dwfl_Line *line = dwfl_module_getsrc(module, ip);
    if (!line)
        return res;

    int line_pos = 0;
    int col_pos = 0;

    GElf_Sym sym;
    GElf_Word section_index;

    const char *filename = dwfl_lineinfo(line, &ip, &line_pos, &col_pos, nullptr, nullptr);
    const char *symbol_name = dwfl_module_addrsym(module, ip, &sym, &section_index);

    res = SourceInfo{
        .filename = filename,
        .so_symbol_name = symbol_name,
        .col_pos = col_pos,
        .line_pos = line_pos,
    };

    return res;
}
