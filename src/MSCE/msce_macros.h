#ifndef MSCE_MACORS_HPP_
/**
 * This file defines general macros, like serialization and relection -related.
 * For specific macros, like system, prototype etc. refer to system prototype etc. header files corespondingly.
 */
#define MSCE_MACORS_HPP_

#include <iostream>
#include <any>

#include <cereal/cereal.hpp>
#include <cereal/access.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/common.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/unordered_set.hpp>
#include <cereal/types/variant.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>

#include <boost/preprocessor.hpp>

namespace msce
{
    class PrototypeManager;

    template <typename T>
    struct Registration;
}

namespace msce::macro_helpers
{
    inline std::string format_file_and_line_helper(const std::string &file, const std::string &line)
    {
        return file + ":" + line;
    }
}
#define MSCE_FILE_TRACK_HELPER(file, line) ::msce::macro_helpers::format_file_and_line_helper(file, #line)
#define MSCE_FILE_TRACK_WRAPPER(file, line) MSCE_FILE_TRACK_HELPER(file, line)
#define MSCE_FILE_TRACK_STR() MSCE_FILE_TRACK_WRAPPER(__FILE__, __LINE__)

#pragma region cereal serialization
#define WRAP_NVP(r, data, x) CEREAL_NVP(x)
#define ENUM_LOAD_IF_HELPER(Enum, value) else if (s == #value) e = Enum::value;
#define WRAP_ENUM_LOAD_IF(r, data, x) ENUM_LOAD_IF_HELPER(data, x)

#define ENUM_SAVE_CASE_HELPER(Enum, value) \
    case Enum::value:                      \
        return #value;
#define WRAP_ENUM_SAVE_CASE(r, data, x) ENUM_SAVE_CASE_HELPER(data, x)

/// @brief Generates serialize() method for object it's called in for cereal to use.
///  Needs to be placed inside a class defenition.
///  This macro also adds cereal::access as friend class.
/// @param ... Names of members that need to be serialized.
#define MSCE_CEREAL_GENERATE_SERIALIZE_METHODS(...)                                                        \
    friend class ::cereal::access;                                                                         \
    template <class Archive> /**  @brief Cereal's save function. Refer to their docs for more info.*/      \
    void save(Archive &ar) const                                                                           \
    {                                                                                                      \
        ar(BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(WRAP_NVP, _, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)))); \
    }                                                                                                      \
    /**  @brief Cereal's load function. Refer to their docs for more info.*/                               \
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
    /**  @brief Cereal's save function. Refer to their docs for more info.*/                               \
    template <class Archive>                                                                               \
    void save(Archive &ar) const                                                                           \
    {                                                                                                      \
        ar(cereal::base_class<ParentClass>(this),                                                          \
           BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(WRAP_NVP, _, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)))); \
    }                                                                                                      \
    /**  @brief Cereal's load function. Refer to their docs for more info.*/                               \
    template <class Archive>                                                                               \
    void load(Archive &ar)                                                                                 \
    {                                                                                                      \
        ar(cereal::base_class<ParentClass>(this),                                                          \
           BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(WRAP_NVP, _, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)))); \
    }

#define MSCE_CEREAL_GENERATE_ENUM_SERIALIZE_METHODS(Enum, ...)                                           \
    namespace cereal                                                                                     \
    {                                                                                                    \
        /**  @brief Cereal's save function. Refer to their docs for more info.*/                         \
        template <class Archive>                                                                         \
        std::string save_minimal(Archive const &, Enum const &e)                                         \
        {                                                                                                \
            switch (e)                                                                                   \
            {                                                                                            \
                BOOST_PP_SEQ_FOR_EACH(WRAP_ENUM_SAVE_CASE, Enum, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)); \
            }                                                                                            \
            return "Unknown enum value";                                                                 \
        }                                                                                                \
        /**  @brief Cereal's load function. Refer to their docs for more info.*/                         \
        template <class Archive>                                                                         \
        void load_minimal(Archive const &, Enum &e, std::string const &s)                                \
        {                                                                                                \
            if (false)                                                                                   \
                return;                                                                                  \
            BOOST_PP_SEQ_FOR_EACH(WRAP_ENUM_LOAD_IF, Enum, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__));       \
        }                                                                                                \
    }
#pragma endregion
#endif // MSCE_MACORS_HPP_