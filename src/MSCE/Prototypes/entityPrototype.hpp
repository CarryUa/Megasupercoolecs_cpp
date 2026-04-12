#ifndef MSCE_ENTITY_PROTOTYPE_HPP_
#define MSCE_ENTITY_PROTOTYPE_HPP_
#include <MSCE/Prototypes/prototype.hpp>
#include <MSCE/ECS/entity.h>

namespace msce
{
    struct EntityPrototype : public IPrototype
    {
        std::vector<std::shared_ptr<IComponent>> components;
        MSCE_CEREAL_GENERATE_DERIVED_SERIALIZE_METHODS(IPrototype, components);
    };
}
MSCE_CEREAL_REGISTER_PROTOTYPE(EntityPrototype)

#endif // MSCE_ENTITY_PROTOTYPE_HPP_
