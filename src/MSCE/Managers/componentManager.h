#ifndef _MSCE_COMPONENT_MANAGER_H_
#define _MSCE_COMPONENT_MANAGER_H_
#include <vector>
#include <memory>
#include <MSCE/Types/Collections/smartUniquePointerList.hpp>
#include <MSCE/Types/Collections/handle.hpp>
#include <MSCE/Types/singleton.hpp>
#include <MSCE/ECS/component.h>
#include <MSCE/logger.h>

namespace msce
{
    /**
     * @brief This is @ref msce::SmartHandle alias for component manager.
     * @tparam TComp the type of component stored under the handle.
     */
    template <typename TComp>
    using ComponentHandle = SmartHandle<SmartUniquePointerList<IComponent>, TComp>;

    /**
     * @brief Manages the component lifetime. You should always use it for component creation/cloning/deletion.
     */
    class ComponentManager : public Singleton<ComponentManager>
    {
    private:
        /// @brief Component storage
        SmartUniquePointerList<IComponent> components_;

    public:
        ComponentManager();

        /**
         * @brief Creates new component.
         * @tparam TComp Type of component to be created.
         * @return @ref msce::ComponentHandle of newly created component.
         */
        template <typename TComp>
        ComponentHandle<TComp> create_component();

        /**
         * @brief Clones new component.
         * @param other @ref msce::ComponentHandle of component that will be cloned.
         * @tparam TComp Type of component to be cloned.
         * @return @ref msce::ComponentHandle of cloned component.
         */
        template <typename TComp>
        ComponentHandle<TComp> clone_component(ComponentHandle<TComp> other);

        /**
         * @brief Gets the component under given id.
         * @param id The id of the component.
         * @tparam TComp Type of component.
         * @return @ref msce::ComponentHandle of component or nullhandle.
         */
        template <typename TComp>
        ComponentHandle<TComp> get_component(std::size_t id) const;

        /**
         * @brief Enumerates all components of given type.
         * @tparam TComp Type of components to be enumerated.
         * @return std::vector of components.
         * @note Note that it will include any component dynamic-castable to TComp.
         */
        template <typename TComp>
        std::vector<ComponentHandle<TComp>> get_all_components_of_type();

        /**
         * @brief Destroys the component provided.
         * @tparam TComp Type of component to be destroyed. It's implicit from @ref msce::ComponentHandle param, and doesn't matter for destruction.
         * @param comp The @ref msce::ComponentHandle of component to be destroyed.
         */
        template <typename TComp>
        void destroy_component(ComponentHandle<TComp> comp);

        /**
         * @brief Destroys the component with protided id.
         * @param id The id of component to be destroyed.
         */
        void destroy_component(size_t id);

        /**
         * @return Alive components count.
         */
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