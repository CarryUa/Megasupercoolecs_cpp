#ifndef _MSCE_COMPONENT_MANAGER_H_
#define _MSCE_COMPONENT_MANAGER_H_
#include <vector>
#include <memory>
#include <MSCE/Common/Interfaces/Singleton.hpp>
#include <MSCE/ECS/component.h>
#include <MSCE/logger.h>

namespace msce
{
    // class IComponent;

    class ComponentManager : public Singleton<ComponentManager>
    {
    private:
        /// @brief This value controls how far should pointer be from end of the vector to be reused.
        size_t min_distance_to_reuse_ = 5;
        size_t alive_components_count_ = 0;

        std::vector<std::unique_ptr<IComponent>>
            _components = std::vector<std::unique_ptr<IComponent>>();

        size_t smallest_available_id_ = 0;

        void update_smallest_available_id();

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
        TComp *p = new TComp(smallest_available_id_);
        this->_components[smallest_available_id_].reset(dynamic_cast<IComponent *>(p));

        alive_components_count_++;

        update_smallest_available_id();
        return p;
    }

    template <typename TComp>
    inline TComp *ComponentManager::clone_component(TComp *other)
    {
        IComponent *cloned = other->clone();
        this->_components[smallest_available_id_].reset(cloned);
        cloned->id_ = smallest_available_id_;

        alive_components_count_++;

        update_smallest_available_id();
        return dynamic_cast<TComp *>(cloned);
    }

    template <typename TComp>
    inline TComp *ComponentManager::get_component(std::size_t id) const
    {
        if (_components.size() <= id)
            return nullptr;

        return dynamic_cast<TComp *>(_components[id].get());
    }
    template <typename TComp>
    inline std::vector<TComp *> ComponentManager::get_all_components_of_type()
    {
        std::vector<TComp *> return_vec;
        for (auto &comp : this->_components)
        {
            TComp *result = dynamic_cast<TComp *>(comp.get());
            if (result != nullptr)
                return_vec.push_back(result);
        }

        return return_vec;
    }
}

#endif //_MSCE_COMPONENT_MANAGER_H_