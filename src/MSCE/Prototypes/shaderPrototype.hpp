#ifndef MSCE_SHADER_PROTOTYPE_H_
#define MSCE_SHADER_PROTOTYPE_H_
#include <MSCE/Prototypes/prototype.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

namespace msce
{
    enum class ShaderType
    {
        Invalid = 0,
        VertexShader = GL_VERTEX_SHADER,
        FragmentShader = GL_FRAGMENT_SHADER,

    };
    enum class GLType
    {
        Short = GL_SHORT,
        UShort = GL_UNSIGNED_SHORT,
        Byte = GL_BYTE,
        UByte = GL_UNSIGNED_BYTE,
        Int = GL_INT,
        UInt = GL_UNSIGNED_INT,
        Float = GL_FLOAT,
        Double = GL_DOUBLE,
        IntVec2 = GL_INT_VEC2,
        UIntVec2 = GL_UNSIGNED_INT_VEC2,
        BoolVec2 = GL_BOOL_VEC2,
        FloatVec2 = GL_FLOAT_VEC2,
        IntVec3 = GL_INT_VEC3,
        UIntVec3 = GL_UNSIGNED_INT_VEC3,
        BoolVec3 = GL_BOOL_VEC3,
        FloatVec3 = GL_FLOAT_VEC3,

    };
    struct ShaderPrototype : public IPrototype
    {
        ShaderType type = ShaderType::Invalid;
        std::string source_path = "";
        std::unordered_map<std::string, GLType> attribs = {};
        std::unordered_map<std::string, GLType> uniforms = {};
        MSCE_DEFINE_PROTOTYPE(ShaderPrototype, type, source_path, attribs, uniforms)
    };
}

MSCE_REGISTER_PROTOTYPE(msce::ShaderPrototype, ShaderPrototype)

#endif // MSCE_SHADER_PROTOTYPE_H_