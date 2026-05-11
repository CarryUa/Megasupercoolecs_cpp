#ifndef _MSCE_COMPONENT_MANAGER_H_
#define _MSCE_COMPONENT_MANAGER_H_
#include <vector>
#include <memory>
#include <MSCE/Common/smartPointerList.h>
#include <MSCE/Common/Interfaces/Singleton.hpp>
#include <MSCE/ECS/component.h>
#include <MSCE/logger.h>

namespace msce
{
    // class IComponent;

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
        TComp *create_component();

        template <typename TComp>
        TComp *clone_component(TComp *other);

        /// @brief Returns component with provided id.
        /// @tparam TComp Expected type of component.
        /// @return Pointer to newly created component or nullptr.
        template <typename TComp>
        TComp *get_component(std::size_t id) const;

        /// @brief Fetches all components of given type.
        /// @tparam TComp Type of component to be created
        /// @return Pointer to newly created component.
        template <typename TComp>
        std::vector<TComp *> get_all_components_of_type();

        /// @brief Fetches all components of given type.
        /// @return Pointer to newly created component.
        void destroy_component(IComponent *comp);

        /// @brief Fetches all components of given type.
        /// @return Pointer to newly created component.
        void destroy_component(size_t id);

        std::size_t count() const noexcept;
    };

    template <typename TComp>
    inline TComp *ComponentManager::create_component()
    {
        static_assert(std::is_base_of_v<IComponent, TComp>,
                      "TComp must derive from IComponent");
        TComp *c = new TComp();
        this->components_.insert(c);
        return c;
    }

    template <typename TComp>
    inline TComp *ComponentManager::clone_component(TComp *other)
    {
        IComponent *cloned = other->clone();
        this->components_.insert(cloned);
        return dynamic_cast<TComp *>(cloned);
    }

    template <typename TComp>
    inline TComp *ComponentManager::get_component(std::size_t id) const
    {
        return dynamic_cast<TComp *>(this->components_.get_item(id));
    }
    template <typename TComp>
    inline std::vector<TComp *> ComponentManager::get_all_components_of_type()
    {
        std::vector<TComp *> return_vec;
        for (auto *comp : this->components_)
        {

            TComp *result = dynamic_cast<TComp *>(comp);
            if (result != nullptr)
                return_vec.push_back(result);
        }

        return return_vec;
    }
}

#endif //_MSCE_COMPONENT_MANAGER_H_