#include "shaders.h"

#include <MSCE/Managers/eventManager.h>
#include <MSCE/Managers/prototypeManager.h>

namespace
{
    std::string read_shader_source_file(const std::string &fn)
    {
        std::ifstream f(fn);
        if (!f)
            throw std::invalid_argument(std::format("File at '{}' not found.", fn));

        std::stringstream ss;
        ss << f.rdbuf();

        return ss.str();
    }
}

void msce::Shader::ShaderData::delete_stage()
{
    if (stage_handle == 0)
        return;

    glDeleteShader(stage_handle);
    stage_handle = 0;
}

bool msce::Shader::ShaderData::compile(const std::string &source_path)
{
    this->stage_handle = glCreateShader((GLenum)this->stage_type);
    std::string source = read_shader_source_file(source_path);

    const char *source_buff = source.c_str();
    if (!source_buff)
        return false;

    glShaderSource(stage_handle, 1, &source_buff, NULL);

    glCompileShader(stage_handle);
    GLint suc;
    glGetShaderiv(stage_handle, GL_COMPILE_STATUS, &suc);
    if (suc == GL_FALSE)
    {
        GLint log_len = 0;
        glGetShaderiv(stage_handle, GL_INFO_LOG_LENGTH, &log_len);

        std::string msg(log_len, '\0');
        glGetShaderInfoLog(stage_handle, log_len, nullptr, &msg[0]);
        logger.log_error("Failed to compile shader at '{}'. Compiler error:\n{}", source_path, msg);
    }

    return suc == GL_TRUE;
}

bool msce::Shader::ShaderData::recompile()
{
    this->delete_stage();

    if (!this->compile(stage_source_path))
    {
        this->delete_stage();
        return false;
    }

    return true;
}

bool msce::Shader::ShaderData::is_ready() const
{
    return stage_handle != 0;
}

msce::Shader::ShaderData::ShaderData(const std::string &source_path, ShaderStageType type, const std::string &sh_name) : logger(Logger(std::format("Shader('{}' {} stage)", sh_name, (type == ShaderStageType::VERTEX ? "VERTEX" : "FRAGMENT")))),
                                                                                                                         stage_source_path(source_path)
{
    this->stage_type = type;

    if (!compile(stage_source_path))
        delete_stage();
}

msce::Shader::ShaderData::~ShaderData()
{
    this->delete_stage();
}

GLuint msce::Shader::get_handle() const noexcept
{
    return sh_handle_;
}

msce::Shader::Shader(const ShaderPrototype &proto) : proto_(proto),
                                                     logger_(std::format("Shader('{}')", proto.id)),
                                                     vert_(proto.vertex_source_path, ShaderStageType::VERTEX, proto.id),
                                                     frag_(proto.fragment_source_path, ShaderStageType::FRAGMENT, proto.id)

{
    this->sh_handle_ = glCreateProgram();
    glAttachShader(sh_handle_, vert_.stage_handle);
    glAttachShader(sh_handle_, frag_.stage_handle);

    glLinkProgram(sh_handle_);

    glDetachShader(sh_handle_, vert_.stage_handle);
    glDetachShader(sh_handle_, frag_.stage_handle);

    GLint suc = 0;
    glGetProgramiv(sh_handle_, GL_LINK_STATUS, &suc);

    if (suc == GL_FALSE)
    {
        GLint len;
        glGetProgramiv(sh_handle_, GL_INFO_LOG_LENGTH, &len);
        std::string msg(len, '\0');

        glGetProgramInfoLog(sh_handle_, len, nullptr, msg.data());
        logger_.log_error("Failed to link shader '{}'. Reason:\n{}", proto_.id, msg);
        return;
    }
}

bool msce::Shader::is_ready() const noexcept
{
    return this->frag_.is_ready() && this->frag_.is_ready();
}
