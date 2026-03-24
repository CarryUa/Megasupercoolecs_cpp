#ifndef MSCE_ENTITY_H_
#define MSCE_ENTITY_H_
#include <vector>
#include <MSCE/ECS/component.h>

namespace msce
{
    class Entity
    {
    private:
        std::vector<IComponent *> _components;

    public:
        size_t get_entity_id();

        template <typename TComp>
        bool has_component()
        {
            for (auto *comp : _components)
            {
                if (dynamic_cast<TComp *>(comp) != nullptr)
                    return true;
            }
            return false;
        }

        bool has_component(IComponent *component);

        template <typename TComp>
        TComp *get_component()
        {
            for (auto &comp : _components)
            {
                TComp *result = dynamic_cast<TComp *>(comp);
                if (result != nullptr)
                    return result;
            }
            return nullptr;
        }

        template <typename TComp>
        std::vector<TComp *> get_components()
        {
            std::vector<TComp *> result;
            for (auto &comp : _components)
            {
                TComp *casted_comp = dynamic_cast<TComp *>(comp);
                if (casted_comp != nullptr)
                {
                    result.push_back(casted_comp);
                }
            }
            return result;
        }

        /// @brief Attaches a component to an entity if there's no component of same type.
        template <typename TComp>
        void attach_component(TComp *comp)
        {
            if (!has_component<TComp>())
            {
                _components.push_back(comp);
            }
        }

        /// @brief Attaches a component to an entity if there's no component of same type.
        template <typename TComp>
        void detach_component(TComp *comp)
        {
            for (size_t i = 0; i < _components.size(); ++i)
            {
                if (_components[i] != comp)
                    continue;

                if (dynamic_cast<TComp *>(_components[i]) == nullptr)
                    continue;

                _components.erase(_components.begin() + i);
                return;
            }
        }
        /// @brief Attaches a component to an entity if there's no component of same type.
        template <typename TComp>
        void detach_component()
        {
            for (size_t i = 0; i < _components.size(); ++i)
            {
                if (dynamic_cast<TComp *>(_components[i]) != nullptr)
                {
                    _components.erase(_components.begin() + i);
                    return;
                }
            }
        }

        /// @brief Attaches a component to an entity. Overwtites existing component of same time
        template <typename TComp>
        void force_attach_component(TComp *comp)
        {

            _components.push_back(comp);
        }
    };
}

#endif // MSCE_ENTITY_H_