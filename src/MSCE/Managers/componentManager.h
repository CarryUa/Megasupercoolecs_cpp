#ifndef _MSCE_COMPONENT_MANAGER_H_
#define _MSCE_COMPONENT_MANAGER_H_
#include <vector>
#include <memory>
#include <MSCE/Common/smartUniquePointerList.hpp>
#include <MSCE/Common/handle.hpp>
#include <MSCE/Common/Interfaces/Singleton.hpp>
#include <MSCE/ECS/component.h>
#include <MSCE/logger.h>

namespace msce
{
    // class IComponent;

    template <typename TComp>
    using ComponentHandle = SmartHandle<SmartUniquePointerList<IComponent>, TComp>;
    class ComponentManager : public Singleton<ComponentManager>
    {
    private:
        SmartUniquePointerList<IComponent> components_;

    public:
        ComponentManager();

        /// @brief Creates new instance of component of given type.
        /// @tparam TComp Type of component to be created
        /// @return Pointer to newly created component.
        template <typename TComp>
        ComponentHandle<TComp> create_component();

        template <typename TComp>
        ComponentHandle<TComp> clone_component(ComponentHandle<TComp> other);

        /// @brief Returns component with provided id.
        /// @tparam TComp Expected type of component.
        /// @return Pointer to newly created component or nullptr.
        template <typename TComp>
        ComponentHandle<TComp> get_component(std::size_t id) const;

        /// @brief Fetches all components of given type.
        /// @tparam TComp Type of component to be created
        /// @return Pointer to newly created component.
        template <typename TComp>
        std::vector<ComponentHandle<TComp>> get_all_components_of_type();

        /// @brief Fetches all components of given type.
        /// @return Pointer to newly created component.
        template <typename TComp>
        void destroy_component(ComponentHandle<TComp> comp);

        /// @brief Fetches all components of given type.
        /// @return Pointer to newly created component.
        void destroy_component(size_t id);

        std::size_t count() const noexcept;
    };

    template <typename TComp>
    inline ComponentHandle<TComp> ComponentManager::create_component()
    {
        static_assert(std::is_base_of_v<IComponent, TComp>,
                      "TComp must derive from IComponent");
        auto handle = this->components_.insert(dynamic_cast<IComponent *>(new TComp()));
        return static_cast<ComponentHandle<TComp>>(handle);
    }

    template <typename TComp>
    inline ComponentHandle<TComp> ComponentManager::clone_component(ComponentHandle<TComp> other)
    {
        IComponent *cloned = other->clone();
        return static_cast<ComponentHandle<TComp>>(this->components_.insert(cloned));
    }

    template <typename TComp>
    inline ComponentHandle<TComp> ComponentManager::get_component(std::size_t id) const
    {
        return static_cast<ComponentHandle<TComp>>(this->components_.get_item(id));
    }
    template <typename TComp>
    inline std::vector<ComponentHandle<TComp>> ComponentManager::get_all_components_of_type()
    {
        std::vector<ComponentHandle<TComp>> return_vec;
        for (auto comp : this->components_)
        {
            TComp *result = dynamic_cast<TComp *>(comp.get());
            if (result != nullptr)
                return_vec.push_back(static_cast<ComponentHandle<TComp>>(comp));
        }

        return return_vec;
    }
    template <typename TComp>
    inline void ComponentManager::destroy_component(ComponentHandle<TComp> comp)
    {
        this->destroy_component(comp.get_index());
    }
}

#endif //_MSCE_COMPONENT_MANAGER_H_