#ifndef MSCE_I_HAS_ID_
#define MSCE_I_HAS_ID_
#include <cstddef>

namespace msce
{
    template <typename TPointedTo>
    struct SmartUniquePointerList;

    class IHasIntegerIdMember
    {
        template <typename T>
        friend struct SmartUniquePointerList;

    protected:
        virtual void set_id(size_t) = 0;

    public:
        virtual size_t get_id() const = 0;
        virtual ~IHasIntegerIdMember() = default;
    };
}

#endif // MSCE_I_HAS_ID_