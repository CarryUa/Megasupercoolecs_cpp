#ifndef MSCE_MACORS_HPP_
#define MSCE_MACORS_HPP_
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/common.hpp>
#include <cereal/types/string.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>
#include <boost/preprocessor.hpp>

#pragma region cereal
#define WRAP_NVP(r, data, x) CEREAL_NVP(x)

namespace msce
{
    class PrototypeManager;

    template <typename T>
    struct Registration;

    template <typename TProto>
    void register_prototype(const std::string &name);
}

/// @brief Generates serialize() method for object it's called in for cereal to use.
///  Needs to be placed inside a class defenition.
///  This macro also adds cereal::access as friend class.
/// @param ... Names of members that need to be serialized.
#define MSCE_CEREAL_GENERATE_SERIALIZE_METHODS(...)                                                        \
    friend class ::cereal::access;                                                                         \
    template <class Archive>                                                                               \
    void save(Archive &ar) const                                                                           \
    {                                                                                                      \
        ar(BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(WRAP_NVP, _, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)))); \
    }                                                                                                      \
    template <class Archive>                                                                               \
    void load(Archive &ar)                                                                                 \
    {                                                                                                      \
        ar(BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(WRAP_NVP, _, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)))); \
    }

/// @brief Generates serialize() method for object it's called in for cereal to use. Also takes into account objects parent.
///  Needs to be placed inside a class defenition.
///  This macro also adds cereal::access as friend class.
/// @param ParentClass Name of the parent class type, whos members also need serialization.
/// @param ... Names of members that need to be serialized.
#define MSCE_CEREAL_GENERATE_DERIVED_SERIALIZE_METHODS(ParentClass, ...)                                   \
    friend class ::cereal::access;                                                                         \
    template <class Archive>                                                                               \
    void save(Archive &ar) const                                                                           \
    {                                                                                                      \
        ar(cereal::base_class<ParentClass>(this),                                                          \
           BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(WRAP_NVP, _, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)))); \
    }                                                                                                      \
    template <class Archive>                                                                               \
    void load(Archive &ar)                                                                                 \
    {                                                                                                      \
        ar(cereal::base_class<ParentClass>(this),                                                          \
           BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(WRAP_NVP, _, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)))); \
    }

#define MSCE_CEREAL_REGISTER_PROTOTYPE(Type, Name)                 \
    CEREAL_REGISTER_TYPE(Type)                                     \
    CEREAL_REGISTER_POLYMORPHIC_RELATION(::msce::IPrototype, Type) \
    namespace msce                                                 \
    {                                                              \
        template <>                                                \
        struct Registration<Type>                                  \
        {                                                          \
            Registration()                                         \
            {                                                      \
                register_prototype<Type>(#Name);                   \
            }                                                      \
        };                                                         \
        inline Registration<Type> registered_##Name;               \
    }

#pragma endregion

#endif // MSCE_MACORS_HPP_