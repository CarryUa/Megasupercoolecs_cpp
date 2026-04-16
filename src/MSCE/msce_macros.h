#ifndef MSCE_MACORS_HPP_
#define MSCE_MACORS_HPP_

#include <iostream>
#include <any>

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

#pragma region pseudo reflection
template <typename T, typename U>
static constexpr const std::type_info &get_member_type(U T::*)
{
    return typeid(U);
}

#define GET_FIELD_NAME_TYPE_PAIR_HELPER(r, data, x) \
    std::pair(std::string_view(#x), std::reference_wrapper<const std::type_info>(get_member_type(&data::x)))

#define MSCE_SET_FIELD_IMPL(x)                            \
    if (name == #x)                                       \
    {                                                     \
        std::any a = value;                               \
        if (a.type() != typeid(this->x))                  \
        {                                                 \
            std::cerr << "Wrong type for "                \
                      << typeid(this).name()              \
                      << " object's '"                    \
                      << name                             \
                      << "' field. Expected '"            \
                      << typeid(this->x).name()           \
                      << "' but got '" << a.type().name() \
                      << "'!" << std::endl;               \
            return;                                       \
        }                                                 \
        this->x = std::any_cast<decltype(this->x)>(a);    \
    }
#define MSCE_SET_FIELD_WRAPPER(r, data, x) MSCE_SET_FIELD_IMPL(x)

#define MSCE_GET_FIELD_IMPL(x)                                    \
    if (name == #x)                                               \
    {                                                             \
        if (typeid(this->x) != typeid(T))                         \
        {                                                         \
            std::cerr << "Wrong type for "                        \
                      << typeid(this).name()                      \
                      << " object's '"                            \
                      << name                                     \
                      << "' field. Expected '"                    \
                      << typeid(this->x).name()                   \
                      << "' but got '" << typeid(T).name()        \
                      << "'!" << std::endl;                       \
            throw std::runtime_error("Get field type mismatch."); \
        }                                                         \
        std::any a = this->x;                                     \
        return std::any_cast<T>(a);                               \
    }
#define MSCE_GET_FIELD_WRAPPER(r, data, x) MSCE_GET_FIELD_IMPL(x)

#define MSCE_GENERATE_REFLECTION_METHODS(ClassType, ...)                                                                                       \
    static std::unordered_map<std::string_view, std::reference_wrapper<const std::type_info>> get_field_name_type_pairs_static()               \
    {                                                                                                                                          \
        return {BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(GET_FIELD_NAME_TYPE_PAIR_HELPER, ClassType, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)))}; \
    }                                                                                                                                          \
    virtual std::unordered_map<std::string_view, std::reference_wrapper<const std::type_info>> get_field_name_type_pairs() const               \
    {                                                                                                                                          \
        return ClassType::get_field_name_type_pairs_static();                                                                                  \
    }                                                                                                                                          \
    template <typename T>                                                                                                                      \
    void set_field(const std::string &name, T value) noexcept                                                                                  \
    {                                                                                                                                          \
        BOOST_PP_SEQ_FOR_EACH(MSCE_SET_FIELD_WRAPPER, _, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))                                                \
    }                                                                                                                                          \
    template <typename T>                                                                                                                      \
    T get_field(const std::string &name) const                                                                                                 \
    {                                                                                                                                          \
        BOOST_PP_SEQ_FOR_EACH(MSCE_GET_FIELD_WRAPPER, _, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))                                                \
        throw std::runtime_error("Field '" + name + "' not found");                                                                            \
    }

#pragma endregion

#pragma region cereal serialization
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
public:                                                                                                    \
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

#pragma endregion

#pragma region Prototypes

#define MSCE_DEFINE_PROTOTYPE(Type, ...)                                            \
    MSCE_CEREAL_GENERATE_DERIVED_SERIALIZE_METHODS(::msce::IPrototype, __VA_ARGS__) \
    MSCE_GENERATE_REFLECTION_METHODS(Type, __VA_ARGS__)

#define MSCE_REGISTER_PROTOTYPE(Type, Name)                        \
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