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
#if defined(RELEASE)
using SerializationInArchive = cereal::BinaryInputArchive;
using SerializationOutArchive = cereal::BinaryOutputArchive;
#else
using SerializationInArchive = cereal::JSONInputArchive;
using SerializationOutArchive = cereal::JSONOutputArchive;
#endif

class Serializer
{
public:
  template <typename T>
  static void serialize(const T &obj, std::ostream &result);
  template <typename T> static void deserialize(T &dest, std::istream &source);
};

template <typename T>
inline void Serializer::serialize(const T &obj, std::ostream &result)
{
  SerializationOutArchive ar(result);
  ar(obj);
}
template <typename T>
inline void Serializer::deserialize(T &dest, std::istream &source)
{
  SerializationInArchive ar(source);
  ar(dest);
}
} // namespace msce

#endif