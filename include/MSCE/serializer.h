#ifndef MSCE_SERIALIZER_H_
#define MSCE_SERIALIZER_H_
#include <iostream>

#if defined(RELEASE)
#include <cereal/archives/binary.hpp>
#else
#include <cereal/archives/json.hpp>
#endif

#include <cereal/types/memory.hpp>
namespace msce
{
    class Serializer
    {
    public:
        template <typename T>
        static void serialize(const T &obj, std::ostream &result);
        template <typename T>
        static void deserialize(T &dest, std::istream &source);
    };

    template <typename T>
    inline void Serializer::serialize(const T &obj, std::ostream &result)
    {
#if defined(RELEASE)
        cereal::BinaryOutputArchive ar(result);
#else
        cereal::JSONOutputArchive ar(result);
#endif
        ar(obj);
    }
    template <typename T>
    inline void Serializer::deserialize(T &dest, std::istream &source)
    {
#if defined(RELEASE)
        cereal::BinaryInputArchive ar(source);
#else
        cereal::JSONInputArchive ar(source);
#endif
        ar(dest);
    }
}

#endif