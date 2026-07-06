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

        MSCE_DEFINE_PROTOTYPE(::msce::ShaderPrototype, vertex_source_path, fragment_source_path)
    };
}
MSCE_REGISTER_PROTOTYPE(::msce::ShaderPrototype, ShaderPrototype, vertex_source_path, fragment_source_path)

#endif // MSCE_SHADER_PROTOTYPE_H_