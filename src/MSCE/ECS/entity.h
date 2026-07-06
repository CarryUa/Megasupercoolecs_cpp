#ifndef MSCE_ENTITY_H_
#define MSCE_ENTITY_H_
#include <vector>
#include <MSCE/ECS/component.h>
#include <MSCE/Managers/componentManager.h>
#include <MSCE/Types/Collections/handle.hpp>

namespace msce
{
    template <typename T>
    class SmartUniquePointerList;
    class EntityManager;

    /**
     * @brief Class representing an entity in ECS architecture. Containt list of @ref msce::ComponentHandle 's attached to it.
     */
    class Entity
    {
    private:
        friend class EntityManager;

        template <typename TComp>
        using ComponentHandle = SmartHandle<SmartUniquePointerList<IComponent>, TComp>;

        /**
         * @brief List of @ref msce::ComponentHandle 's attached to this entity.
         */
        std::vector<ComponentHandle<IComponent>> components_ = {};
        size_t entity_id_ = 0;

    public:
        /**
         * @tparam TComp type of component to check.
         * @retval true if entity has component of type TComp attached.
         * @retval false if entity does not have component of type TComp attached.
         */
        template <typename TComp>
        bool has_component() const noexcept;

        /**
         * @param @ref msce::ComponentHandle of component to check.
         * @tparam TComp type of component to be checked.
         * @retval true if entity has this exact component attached.
         * @retval false if entity does not have this exact component attached.
         */
        template <typename TComp>
        bool has_component(ComponentHandle<TComp> component) noexcept;

        /**
         * @tparam TComp Type of component to get.
         * @retval @ref msce::ComponentHandle of component of type TComp - if said component is attached.
         * @retval @ref nullhandle - if it is not attached.
         */
        template <typename TComp>
        ComponentHandle<TComp> get_component() const noexcept;

        /**
         * @tparam TComp Type of components to get.
         * @returns std::vector of @ref msce::ComponentHandle 's of components of type TComp attached to this entity.
         */
        template <typename TComp>
        std::vector<ComponentHandle<TComp>> get_components() const noexcept;

        /**
         * @brief Attaches a component to the entity, if component of same type isn't already attached.
         *
         * @tparam TComp Type of component to be attached. Implicit from @ref msce::ComponentHandle
         * @param comp @ref msce::ComponentHandle of component to be attached.
         *
         * @note This will not do anything if component of type TComp is already attached.
         * @note If you want to overwrite the component, see @ref msce::Entity::force_attach_component()
         */
        template <typename TComp>
        void attach_component(ComponentHandle<TComp> comp) noexcept;

        /**
         * @brief Detaches the component of type TComp from this entity, if said component is attached.
         */
        template <typename TComp>
        void detach_component() noexcept;

        /**
         * @brief Attaches a component to the entity regardless. This will detach and destroy the component of TComp previously stored, and attached new instead.
         * @tparam TComp Type of component to be attached. Implicit from @ref msce::ComponentHandle.
         * @param comp The component to be force-attahed.
         *
         * @warning Previous component of type TComp will be destroyed!
         */
        template <typename TComp>
        void force_attach_component(ComponentHandle<TComp> comp) noexcept;
    };
    template <typename TComp>
    inline bool Entity::has_component() const noexcept
    {
        for (auto comp : components_)
        {
            if (dynamic_cast<TComp *>(comp.get()) != nullptr)
                return true;
        }
        return false;
    }
    template <typename TComp>
    inline bool Entity::has_component(ComponentHandle<TComp> component) noexcept
    {
        static_assert(std::is_base_of_v<IComponent, TComp>, "TComp must inherrit from IComponent");

        if (component == nullptr)
            return false;

        for (auto &comp : components_)
        {
            auto cast_handle = static_cast<ComponentHandle<TComp>>(comp);

            if (cast_handle == component)
                return true;
        }
        return false;
    }
    template <typename TComp>
    inline SmartHandle<SmartUniquePointerList<IComponent>, TComp> Entity::get_component() const noexcept
    {
        for (auto &comp : components_)
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
        for (auto &comp : components_)
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
        static_assert(std::is_base_of_v<IComponent, TComp>, "TComp must inherrit from IComponent");

        if (!has_component<TComp>())
        {
            comp->set_owner(this);
            components_.push_back(static_cast<ComponentHandle<IComponent>>(comp));
        }
    }
    template <typename TComp>
    inline void Entity::force_attach_component(ComponentHandle<TComp> comp) noexcept
    {
        static_assert(std::is_base_of_v<IComponent, TComp>, "TComp must inherrit from IComponent");

        if (has_component<TComp>())
            detach_component<TComp>();

        comp->set_owner(this);
        components_.push_back(static_cast<ComponentHandle<IComponent>>(comp));
    }
    template <typename TComp>
    inline void Entity::detach_component() noexcept
    {
        for (size_t i = 0; i < components_.size(); ++i)
        {
            if (dynamic_cast<TComp *>(components_[i].get()) == nullptr)
                continue;

            ComponentManager::instance->destroy_component(components_[i]);
            components_.erase(components_.begin() + i);
            return;
        }
    }
}

#endif // MSCE_ENTITY_H_