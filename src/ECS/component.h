#ifndef _MSCE_COMPONENT_H_
#define _MSCE_COMPONENT_H_
#include <iostream>
namespace msce
{
    class Component
    {
    protected:
        size_t _id;

    public:
        Component(size_t id);
        virtual ~Component() = default;
        size_t get_id() const;
    };
}

#endif // _MSCE_COMPONENT_H_