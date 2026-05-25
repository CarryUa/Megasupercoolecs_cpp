#ifndef MSCE_ENTITY_PROTOTYPE_HPP_
#define MSCE_ENTITY_PROTOTYPE_HPP_
#include <MSCE/Prototypes/prototype.hpp>
#include <MSCE/ECS/entity.h>

namespace msce
{
    /// @brief Prototype for all entities. Think of it as of Unity prefabs maybe?
    struct EntityPrototype : public IPrototype
    {
        /// @brief List of components for serialization.
        std::vector<std::shared_ptr<IComponent>> components;
        MSCE_DEFINE_PROTOTYPE(EntityPrototype, components)
    };
}
MSCE_REGISTER_PROTOTYPE(msce::EntityPrototype, EntityPrototype)

#endif // MSCE_ENTITY_PROTOTYPE_HPP_
