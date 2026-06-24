#ifndef MSCE_PLATFORM_H_
#define MSCE_PLATFORM_H_
#include <string>
#include <MSCE/logger.h>

namespace msce
{

    /**
     * @brief Cross-platform interface for platform-dependent things.
     */
    class Platform
    {
    private:
    public:
        struct SourceInfo
        {
            const char *filename;
            const char *so_symbol_name;
            int col_pos;
            int line_pos;
        };
        static std::string demangle(const std::string &mangled, bool log = true) noexcept;
        static std::string get_absolute_executable_path();
        static SourceInfo resolve_shared_object_address_source(void *address);
    };
}

#endif