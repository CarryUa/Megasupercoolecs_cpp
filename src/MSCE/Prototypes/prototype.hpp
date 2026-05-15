#ifndef MSCE_I_PROTOTYPE_H_
#define MSCE_I_PROTOTYPE_H_
#include <string>
#include <MSCE/msce_macros.h>

#pragma region Prototype Macros

#define MSCE_DEFINE_PROTOTYPE(Type, ...)                                            \
    MSCE_CEREAL_GENERATE_DERIVED_SERIALIZE_METHODS(::msce::IPrototype, __VA_ARGS__) \
    MSCE_GENERATE_REFLECTION_METHODS_DERIVED(Type, ::msce::IPrototype, __VA_ARGS__)

#define MSCE_REGISTER_PROTOTYPE(Type, Name)                        \
    CEREAL_REGISTER_TYPE(Type)                                     \
    CEREAL_REGISTER_POLYMORPHIC_RELATION(::msce::IPrototype, Type) \
    namespace msce                                                 \
    {                                                              \
        template <>                                                \
        class Registration<Type>                                   \
        {                                                          \
        public:                                                    \
            [[gnu::used]] Registration()                           \
            {                                                      \
                register_prototype<Type>(#Name);                   \
            }                                                      \
        };                                                         \
        inline Registration<Type> registered_##Name;               \
    }

#pragma endregion

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