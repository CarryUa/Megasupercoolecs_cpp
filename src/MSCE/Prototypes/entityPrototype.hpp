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
CEREAL_REGISTER_TYPE(msce::EntityPrototype)
CEREAL_REGISTER_POLYMORPHIC_RELATION(msce::IPrototype, msce::EntityPrototype)

#endif // MSCE_ENTITY_PROTOTYPE_HPP_
