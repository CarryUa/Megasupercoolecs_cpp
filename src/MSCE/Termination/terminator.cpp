#include "terminator.h"
#include <MSCE/logger.h>
#include <MSCE/Platform/platform.h>
#include <string>

#include <execinfo.h>

namespace
{
    static msce::Logger stacktrace_logger("Stack Trace");
    constexpr const char *library_name = "libmsce";
}

void msce::internal::StackTraceHandler::handle()
{
    constexpr const int max_frames = 32;
    Logger l("Stack Trace");

    void *trace[max_frames];
    int count = backtrace(trace, max_frames);
    if (!trace)
        return;

    char **messages = backtrace_symbols(trace, count);
    if (!messages)
        return;

    l.log_debug("Error stack trace: ");
    int error_trace_start = 0;
    bool prev_had_libstdcpp = false;

    for (int i = 0; i < count; i++)
    {
        std::string msg(messages[i]);
        if (!prev_had_libstdcpp && msg.find("libstdc++") != std::string::npos)
        {
            prev_had_libstdcpp = true;
            continue;
        }

        if (prev_had_libstdcpp && msg.find("libstdc++") == std::string::npos)
        {
            error_trace_start = i;
            break;
        }
    }

    if (error_trace_start == 0)
        return;

    int skipped = 0;
    for (int i = 0; i < count - error_trace_start; i++)
    {
        std::string msg(messages[i + error_trace_start]);

        auto source_info = Platform::resolve_shared_object_address_source(reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(trace[i + error_trace_start]) - 1)); // previous instruction.

        if (i != 0)
            l.log_debug("#{}\t\t| '{}' defined at '{}:{}:{}'", i - skipped, Platform::demangle(source_info.so_symbol_name, false), source_info.filename, source_info.line_pos, source_info.col_pos);
        else
            l.log_debug("#0 (err origin)\t| '{}' defined at '{}:{}:{}'", Platform::demangle(source_info.so_symbol_name, false), source_info.filename, source_info.line_pos, source_info.col_pos);
    }

#if defined(RELEASE)
    l.log_warning("It seems you are in RELEASE build, which has cryptic stack trace. Run DEBUG build to see demangled more human-friendly trace.");
#endif
    l.log_debug("Executable path: {}", Platform::get_absolute_executable_path());
    free(messages);
}

void msce::internal::handle_terminate()
{
    Logger l("Terminator");
    l.log_debug("Program was terminated due to some unhandled error.");
    if (std::current_exception())
    {
        try
        {
            std::rethrow_exception(std::current_exception());
        }
        catch (const std::exception &e)
        {
            l.log_error("Exception was thrown: {}: '{}'", Platform::demangle(std::current_exception().__cxa_exception_type()->name()), e.what());
        }
    }
    StackTraceHandler::handle();
}
