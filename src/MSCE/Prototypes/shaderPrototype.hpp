#ifndef MSCE_SHADER_PROTOTYPE_H_
#define MSCE_SHADER_PROTOTYPE_H_
#include "prototype.hpp"
#include <glad/glad.h>
#include <glfw3.h>
#include <string>

namespace msce
{
    enum class ShaderType
    {
        Invalid = 0,
        VertexShader = GL_VERTEX_SHADER,
        FragmentShader = GL_FRAGMENT_SHADER,
    };
    struct ShaderPrototype : public IPrototype
    {
        ShaderType type = ShaderType::Invalid;
        std::string source_path = "";
        std::unordered_map<std::string, std::reference_wrapper<std::type_info>> attribs = {};
        std::unordered_map<std::string, std::reference_wrapper<std::type_info>> uniforms = {};
        MSCE_DEFINE_PROTOTYPE(ShaderPrototype, type, source_path, attribs, uniforms)
    };
}

MSCE_REGISTER_PROTOTYPE(msce::ShaderPrototype, ShaderPrototype)

#endif // MSCE_SHADER_PROTOTYPE_H_