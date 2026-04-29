#ifndef MSCE_I_PROTOTYPE_H_
#define MSCE_I_PROTOTYPE_H_
#include <string>
#include <MSCE/msce_macros.h>

namespace msce
{
    class IPrototype
    {
    public:
        std::string id = "Invalid default prototype!";

        virtual ~IPrototype() = default;

    public:
        friend class ::cereal::access;
        template <class Archive>
        void save(Archive &ar) const { ar(::cereal::make_nvp("id", id)); }
        template <class Archive>
        void load(Archive &ar) { ar(::cereal::make_nvp("id", id)); }
        MSCE_GENERATE_REFLECTION_METHODS(IPrototype, id)
    };

}
CEREAL_REGISTER_TYPE(msce::IPrototype)

#endif // MSCE_I_PROTOTYPE_H_