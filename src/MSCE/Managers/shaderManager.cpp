#include "shaderManager.h"
#include <MSCE/logger.h>
#include <MSCE/Managers/eventManager.h>

namespace
{
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

    this->shader_handle_ = glCreateShader((GLenum)this->prototype_->shader_type);

    std::string content = "";
    if (!read_shader_source(this->prototype_->source_path, content))
    {
        logger.log_error("Failed to read shader source at '{}'", this->prototype_->source_path);
        return false;
    }

    const char *source_buffer = content.c_str();
    glShaderSource(this->shader_handle_, content.size(), &(source_buffer), NULL);
    glCompileShader(this->shader_handle_);

    return true;
}

void msce::BaseShader::decompile() noexcept
{
    if (this->shader_handle_ == 0)
        return;
}

msce::BaseShader::BaseShader(ShaderPrototype *prototype) noexcept
{
    MSCE_SUBSCRIBE_TO_EVENT_NON_STATIC(RenderStartEvent, this->on_render_start);
    MSCE_SUBSCRIBE_TO_EVENT_NON_STATIC(PreRenderStartEvent, this->on_pre_render_start);

    this->prototype_ = prototype;
}

GLuint msce::BaseShader::get_shader_handle() const noexcept
{
    return this->shader_handle_;
}

void msce::BaseShader::on_pre_render_start(PreRenderStartEvent &ev)
{
}

void msce::BaseShader::on_render_start(RenderStartEvent &ev)
{
    logger.log_debug("Render event triggered for window {}", ev.target_window.title);
}

msce::ShaderManager::ShaderManager()
{
    logger.log_info("Initializing manager...");
}
