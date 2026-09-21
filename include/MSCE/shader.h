#pragma once
#include <concepts>
#include <glad/glad.h>

#include <MSCE/logger.h>
#include <MSCE/BuiltIns/shaderPrototype.hpp>

namespace msce
{
constexpr const char *SHADER_TRANSFORM_POS_UNIFORM_NAME = "tPos";
constexpr const char *SHADER_TRANSFORM_ROT_UNIFORM_NAME = "tRot";
constexpr const char *SHADER_TRANSFORM_SCL_UNIFORM_NAME = "tScl";
constexpr const char *SHADER_TRANSFORM_SIZ_UNIFORM_NAME = "tSiz";
constexpr const char *SHADER_WIN_RESOLUTION_UNIFORM_NAME = "win_resolution";
constexpr const char *SHADER_COLOR_UNIFORM_NAME = "color";
constexpr const char *SHADER_TEXTURE_UNIFORM_NAME = "tex0";
/**
 * @brief Manages openGL shaders-and-programs semantics.
 * @remark An umbrella that covers creating and preparing shaders, as well
 * as linking them to programs.
 */
class Shader
{
public:
  enum class ShaderStageType : GLenum
  {
    VERTEX = GL_VERTEX_SHADER,
    FRAGMENT = GL_FRAGMENT_SHADER,
  };

  GLuint get_handle() const noexcept;

  Shader(const ShaderPrototype &proto);

  std::unordered_map<std::string, GLint> unitorm_locations;

  bool is_ready() const noexcept;

private:
  const Logger logger_;

  GLuint sh_handle_;
  const ShaderPrototype &proto_;

  struct ShaderData
  {
    const Logger logger;
    bool compile(const std::string &source);
    bool recompile();
    void delete_stage();

    bool is_ready() const;

    GLuint stage_handle = 0;
    ShaderStageType stage_type;

    std::string stage_source_path;

    ShaderData(const std::string &source_path, ShaderStageType type,
               const std::string &sh_name);
    ~ShaderData();
  } vert_, frag_;
};
} // namespace msce