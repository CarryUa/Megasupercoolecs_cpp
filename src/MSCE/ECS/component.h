#ifndef _MSCE_COMPONENT_H_
#define _MSCE_COMPONENT_H_
#include <iostream>
#include <MSCE/Managers/componentManager.h>
namespace msce
{
    class IComponent
    {
    public:
        virtual ~IComponent() = default;
        virtual size_t get_id() = 0;
        virtual IComponent *clone() = 0;
    };

    template <typename TComp>
    class BaseComponent : public IComponent
    {
    public:
        size_t get_id() override
        {
            return ComponentManager::instance->get_component_id(dynamic_cast<IComponent *>(this));
        }

        /// @brief This method is used by ComponentManager, dont relly on it for cloning components
        /// @return
        IComponent *clone() override
        {
            return new TComp(static_cast<const TComp &>(*this));
        }
    };
}

#endif // _MSCE_COMPONENT_H_