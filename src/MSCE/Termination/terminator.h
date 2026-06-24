#ifndef MSCE_STRACE_H_
#define MSCE_STRACE_H_
#include <string>
namespace msce::internal
{

    class StackTraceHandler
    {
    private:
    public:
        static void handle();
    };

    void handle_terminate();
}

#endif