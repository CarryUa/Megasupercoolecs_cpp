#ifndef _MSCE_COMPONENT_MANAGER_H_
#define _MSCE_COMPONENT_MANAGER_H_
#include <vector>
#include <memory>
#include <MSCE/Common/Interfaces/Singleton.hpp>

namespace msce
{
    class IComponent;

    class ComponentManager : public Singleton<ComponentManager>
    {
    private:
        std::vector<std::unique_ptr<IComponent>>
            _components;

    public:
        /// @brief Creates new instance of component of given type.
        /// @tparam TComp Type of component to be created
        /// @return Pointer to newly created component.
        template <typename TComp>
        TComp *create_component()
        {

            this->_components.push_back(std::make_unique<TComp>());

            return dynamic_cast<TComp *>(this->_components.back().get());
        }

        template <typename TComp>
        TComp *clone_component(TComp *other)
        {
            IComponent *cloned = other->clone();

            return dynamic_cast<TComp *>(this->_components.back().get());
        }

        /// @brief Returns component with provided id.
        /// @tparam TComp Expected type of component.
        /// @return Pointer to newly created component or nullptr.
        template <typename TComp>
        TComp *get_component(std::size_t id) const
        {
            if (_components.size() <= id)
                return nullptr;

            return dynamic_cast<TComp *>(_components[id].get());
        }

        /// @brief Fetches all components of given type.
        /// @tparam TComp Type of component to be created
        /// @return Pointer to newly created component.
        template <typename TComp>
        std::vector<TComp *> get_components()
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

        void register_component(IComponent *comp);

        /// @brief Fetches all components of given type.
        /// @return Pointer to newly created component.
        void destroy_component(IComponent *comp);

        size_t get_component_id(IComponent *comp);
    };

}

#endif //_MSCE_COMPONENT_MANAGER_H_