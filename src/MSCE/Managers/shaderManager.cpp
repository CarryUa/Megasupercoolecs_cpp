#include "shaderManager.h"
#include <MSCE/logger.h>

namespace
{
    msce::Logger logger("BaseShader");
    bool read_shader_source(const std::string_view &path, std::string &result)
    {
        std::ifstream file(path.data());
        if (!file.is_open())
            return false;

        std::stringstream buffer;
        buffer << file.rdbuf();

        result = buffer.str();
        return true;
    }
}

void msce::BaseShader::set_id(size_t id) noexcept
{
    this->smart_storage_id_ = id;
}

size_t msce::BaseShader::get_id() const noexcept
{
    return this->smart_storage_id_;
}

bool msce::BaseShader::recompile() noexcept
{
    if (this->shader_handle_ != 0)
    {
        glDeleteShader(this->shader_handle_);
    }
    if (this->prototype_ == nullptr)
    {
        logger.log_error("Tried to compile prototype with invalid prototype.");
        return false;
    }

    this->shader_handle_ = glCreateShader((GLenum)this->prototype_->type);

    std::string content = "";
    if (!read_shader_source(this->prototype_->source_path, content))
    {
        logger.log_error("Failed to read shader source at '{}'", this->prototype_->source_path);
        return false;
    }

    const char *source_buffer = content.c_str();
    glShaderSource(this->shader_handle_, content.size(), &(source_buffer), NULL);
    glCompileShader(this->shader_handle_);
}

void msce::BaseShader::decompile() noexcept
{
    if (this->shader_handle_ == 0)
        return;
}

msce::BaseShader::BaseShader(ShaderPrototype *prototype)
{
    this->prototype_ = prototype;
}

GLuint msce::BaseShader::get_shader_handle() const noexcept
{
    return this->shader_handle_;
}
