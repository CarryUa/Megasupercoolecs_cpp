#pragma once
#include <cstdint>
#include <vector>
#include <type_traits>
#include <MSCE/msce_macros.h>

namespace msce
{

class IMesh
{
public:
  virtual ~IMesh() = default;
  virtual const vec2d *get_vertecies() const = 0;
  virtual const vec2d *get_uv() const = 0;
  virtual const uint32_t *get_indecies() const = 0;
  virtual constexpr const uint32_t vertex_count() const = 0;
  virtual constexpr const uint32_t index_count() const = 0;
};

template <uint32_t t_vert_count, uint32_t t_index_count>
struct Mesh : public IMesh
{
public:
  // using index_type = std::conditional_t<
  //     (t_vert_count <= UINT8_MAX), uint8_t,
  //     std::conditional_t<(t_vert_count <= UINT16_MAX), uint16_t, uint32_t>>;

  vec2d vertecies[t_vert_count];
  vec2d texture_uv[t_vert_count];
  uint32_t indecies[t_index_count];

  Mesh(std::initializer_list<vec2d> verts = {},
       std::initializer_list<vec2d> uv = {},
       std::initializer_list<uint32_t> inds = {});

  virtual const vec2d *get_vertecies() const override;
  virtual const uint32_t *get_indecies() const override;
  virtual const vec2d *get_uv() const override;
  virtual constexpr const uint32_t vertex_count() const override;
  virtual constexpr const uint32_t index_count() const override;

  MSCE_CEREAL_GENERATE_SERIALIZE_METHODS(vertecies, indecies)
};
template <uint32_t t_vert_count, uint32_t t_index_count>
inline Mesh<t_vert_count, t_index_count>::Mesh(
    std::initializer_list<vec2d> verts, std::initializer_list<vec2d> uv,
    std::initializer_list<uint32_t> inds)
{
  assert(verts.size() == t_vert_count);
  assert(uv.size() == t_vert_count);
  assert(inds.size() == t_index_count);

  std::copy(verts.begin(), verts.end(), vertecies);
  std::copy(uv.begin(), uv.end(), texture_uv);
  std::copy(inds.begin(), inds.end(), indecies);
}
template <uint32_t t_vert_count, uint32_t t_index_count>
inline const vec2d *Mesh<t_vert_count, t_index_count>::get_vertecies() const
{
  return vertecies;
}
template <uint32_t t_vert_count, uint32_t t_index_count>
inline const uint32_t *Mesh<t_vert_count, t_index_count>::get_indecies() const
{
  return indecies;
}
template <uint32_t t_vert_count, uint32_t t_index_count>
inline const vec2d *Mesh<t_vert_count, t_index_count>::get_uv() const
{
  return texture_uv;
}
template <uint32_t t_vert_count, uint32_t t_index_count>
inline constexpr const uint32_t
Mesh<t_vert_count, t_index_count>::vertex_count() const
{
  return t_vert_count;
}
template <uint32_t t_vert_count, uint32_t t_index_count>
inline constexpr const uint32_t
Mesh<t_vert_count, t_index_count>::index_count() const
{
  return t_index_count;
}
} // namespace msce