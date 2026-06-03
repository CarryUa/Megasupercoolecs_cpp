#ifndef MSCE_SHADER_MANAGER_H_
#define MSCE_SHADER_MANAGER_H_
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <MSCE/Types/singleton.hpp>
#include <MSCE/Prototypes/shaderPrototype.hpp>
#include <MSCE/Events/glEvents.h>

// TODO: Finish this later
namespace msce
{
    class ShaderManager;

    /**
     * @brief Base class of all shaders.
     * @related msce::ShaderManager
     * @related msce::ShaderPrototype
     * @note This auto-subscribes to gl-related events, like @ref msce::RenderStartEvent.
     */
    class BaseShader
    {
    private:
        inline static Logger logger = Logger("BaseShader");

    protected:
        friend class ShaderManager;

        std::size_t smart_storage_id_ = 0;
        GLuint shader_handle_ = 0;
        ShaderPrototype *prototype_ = nullptr;
        bool recompile() noexcept;
        void decompile() noexcept;

        MSCE_GENERATE_REFLECTION_METHODS(BaseShader, smart_storage_id_, shader_handle_, prototype_)

    public:
        BaseShader(ShaderPrototype *prototype) noexcept;
        GLuint get_shader_handle() const noexcept;

        virtual void on_pre_render_start(PreRenderStartEvent &ev);
        virtual void on_render_start(RenderStartEvent &ev);
    };

    class ShaderProgram
    {
    };

    class ShaderManager : public Singleton<ShaderManager>
    {
    private:
        inline static Logger logger = Logger("ShaderManager");
        /// @brief The map of shaders and their users(programs) count
        std::unordered_set<std::unique_ptr<BaseShader>, unsigned int> shaders_;

    public:
        ShaderManager();
    };
}

#endif // MSCE_SHADER_MANAGER_H_