#ifndef MSCE_I_PROTOTYPE_H_
#define MSCE_I_PROTOTYPE_H_
#include <string>
#include <MSCE/msce_macros.hpp>

namespace msce
{
    class IPrototype
    {
    public:
        std::string id;

        virtual ~IPrototype() = default;

        MSCE_CEREAL_GENERATE_SERIALIZE_METHODS(id)
    };

}
CEREAL_REGISTER_TYPE(msce::IPrototype);

#endif // MSCE_I_PROTOTYPE_H_