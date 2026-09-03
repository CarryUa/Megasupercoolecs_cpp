#ifndef MSCE_SHADER_PROTOTYPE_H_
#define MSCE_SHADER_PROTOTYPE_H_
#include <MSCE/msce_macros.h>
#include <MSCE/Prototypes/prototype.hpp>
#include <MSCE/Types/enum.hpp>
#include <MSCE/Types/vector.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

namespace msce
{
struct ShaderPrototype : public IPrototype
{
  std::string vertex_source_path = "";
  std::string fragment_source_path = "";

  std::vector<std::string> vertex_uniforms;
  std::vector<std::string> fragment_uniforms;

  MSCE_DEFINE_PROTOTYPE(::msce::ShaderPrototype, vertex_source_path,
                        fragment_source_path, vertex_uniforms,
                        fragment_uniforms)
};
} // namespace msce
MSCE_REGISTER_PROTOTYPE(::msce::ShaderPrototype, ShaderPrototype,
                        vertex_source_path, fragment_source_path)

#endif // MSCE_SHADER_PROTOTYPE_H_