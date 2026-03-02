#ifndef _MSCE_COMPONENT_MANAGER_H_
#define _MSCE_COMPONENT_MANAGER_H_
#include <vector>
#include <ECS/component.h>
#include <memory>

namespace msce
{
    class ComponentManager
    {
    private:
        std::vector<std::unique_ptr<Component>> _components;

    public:
        /// @brief Creates new instance of component of given type.
        /// @tparam TComp Type of component to be created
        /// @return Pointer to newly created component.
        template <typename TComp>
        TComp *create_component()
        {
            this->_components.push_back(std::make_unique<TComp>(_components.size()));
            return dynamic_cast<TComp *>(_components.back().get());
        }

        /// @brief Returns component with provided id.
        /// @tparam TComp Expected type of component.
        /// @return Pointer to newly created component or nullptr.
        template <typename TComp>
        TComp *get_component(std::size_t id) const;

        /// @brief Fetches all components of given type.
        /// @tparam TComp Type of component to be created
        /// @return Pointer to newly created component.
        template <typename TComp>
        std::vector<TComp *> get_components() const;
    };
}

#endif //_MSCE_COMPONENT_MANAGER_H_