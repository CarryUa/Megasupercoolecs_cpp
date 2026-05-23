#ifndef MSCE_ENTITY_H_
#define MSCE_ENTITY_H_
#include <vector>
#include <MSCE/ECS/component.h>
#include <MSCE/Common/handle.hpp>

namespace msce
{
    template <typename T>
    class SmartUniquePointerList;
    class Entity
    {
    private:
        template <typename TComp>
        using ComponentHandle = SmartHandle<SmartUniquePointerList<IComponent>, TComp>;

        std::vector<ComponentHandle<IComponent>> _components;
        size_t entity_id_;

    public:
        template <typename TComp>
        bool has_component() const noexcept;

        bool has_component(ComponentHandle<IComponent> comp) noexcept;

        template <typename TComp>
        ComponentHandle<TComp> get_component() const noexcept;

        template <typename TComp>
        std::vector<ComponentHandle<TComp>> get_components() const noexcept;

        /// @brief Attaches a component to an entity if there's no component of same type.
        template <typename TComp>
        void attach_component(ComponentHandle<TComp> comp) noexcept;

        /**
         * Detaches the component of type TComp if attached.
         */
        template <typename TComp>
        void detach_component() noexcept;

        /// @brief Attaches a component to an entity. Overwrites existing component of same time
        template <typename TComp>
        void force_attach_component(ComponentHandle<TComp> comp) noexcept;
    };
    template <typename TComp>
    inline bool Entity::has_component() const noexcept
    {
        for (auto comp : _components)
        {
            if (dynamic_cast<TComp *>(comp.get()) != nullptr)
                return true;
        }
        return false;
    }
    template <typename TComp>
    inline SmartHandle<SmartUniquePointerList<IComponent>, TComp> Entity::get_component() const noexcept
    {
        for (auto &comp : _components)
        {
            TComp *result = dynamic_cast<TComp *>(comp.get());
            if (result != nullptr)
                return static_cast<ComponentHandle<TComp>>(comp);
        }
        return ComponentHandle<TComp>::create_nullptr();
    }

    template <typename TComp>
    inline std::vector<SmartHandle<SmartUniquePointerList<IComponent>, TComp>> Entity::get_components() const noexcept
    {
        std::vector<ComponentHandle<TComp>> result;
        for (auto &comp : _components)
        {
            TComp *casted_comp = dynamic_cast<TComp *>(comp.get());
            if (casted_comp != nullptr)
            {
                result.push_back(comp);
            }
        }
        return result;
    }
    template <typename TComp>
    inline void Entity::attach_component(ComponentHandle<TComp> comp) noexcept
    {
        if (!has_component<TComp>())
        {
            _components.push_back(static_cast<ComponentHandle<IComponent>>(comp));
        }
    }
    template <typename TComp>
    inline void Entity::force_attach_component(ComponentHandle<TComp> comp) noexcept
    {
        if (has_component<TComp>())
            detach_component<TComp>();
        _components.push_back(comp);
    }
    template <typename TComp>
    inline void Entity::detach_component() noexcept
    {
        for (size_t i = 0; i < _components.size(); ++i)
        {
            if (dynamic_cast<TComp *>(_components[i].get()) == nullptr)
                continue;

            _components.erase(_components.begin() + i);
            return;
        }
    }
}

#endif // MSCE_ENTITY_H_