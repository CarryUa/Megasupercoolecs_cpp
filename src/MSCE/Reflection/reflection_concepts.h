#ifndef MSCE_REFLECTION_CONCEPTS_H_
#define MSCE_REFLECTION_CONCEPTS_H_
#include <type_traits>
#include <concepts>

namespace msce
{
    class Type;
    template <typename T>
    constexpr const Type &get_reflection_of_type();

    /// @brief T is class or struct.
    template <typename T>
    concept Class = std::is_class_v<T>;

    /// @brief T is class/struct with reflection.
    template <typename T>
    concept ClassWithReflection = Class<T> && requires {
        { T::get_type_info() } -> std::convertible_to<const Type &>;
    };

    template <typename T>
    concept TypeWithReflection = requires {
        { get_reflection_of_type<T>() } -> std::convertible_to<const Type &>;
    };
}

#endif // MSCE_REFLECTION_CONCEPTS_H_