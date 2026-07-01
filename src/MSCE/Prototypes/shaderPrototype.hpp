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
    enum class ShaderType : int
    {
        INVALID = 0,
        VERTEX_SHADER = GL_VERTEX_SHADER,
        FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
    };

    using GLAny = std::variant<char, unsigned char, int, unsigned int, float, double, vec2i, vec2ui, vec2f>;

    struct ShaderPrototype : public IPrototype
    {
        ShaderType shader_type = ShaderType::INVALID;
        std::string source_path = "";

    public:
        MSCE_DEFINE_PROTOTYPE(::msce::ShaderPrototype, shader_type, source_path)
    };
}

MSCE_REGISTER_ENUM(::msce::ShaderType, INVALID, VERTEX_SHADER, FRAGMENT_SHADER)

// MSCE_REGISTER_ENUM(msce::ShaderType, ShaderType, INVALID, VERTEX_SHADER, FRAGMENT_SHADER)
MSCE_REGISTER_PROTOTYPE(::msce::ShaderPrototype, ShaderPrototype, shader_type, source_path)

#endif // MSCE_SHADER_PROTOTYPE_H_