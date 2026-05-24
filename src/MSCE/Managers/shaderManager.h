#ifndef MSCE_SHADER_MANAGER_H_
#define MSCE_SHADER_MANAGER_H_
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <MSCE/Types/singleton.hpp>
#include <MSCE/Prototypes/shaderPrototype.hpp>

// TODO: Finish this later
namespace msce
{
    class ShaderManager;

    /**
     * Base class of all shaders.
     * @warning This class does not have any attribute/uniform manipulation methods. It is derived class responsibility to define and implement those.
     */
    class BaseShader
    {
    protected:
        friend class ShaderManager;

        std::size_t smart_storage_id_ = 0;
        GLuint shader_handle_ = 0;
        ShaderPrototype *prototype_ = nullptr;
        bool recompile() noexcept;
        void decompile() noexcept;

    public:
        BaseShader(ShaderPrototype *prototype) noexcept;
        GLuint get_shader_handle() const noexcept;
    };

    class ShaderManager : public Singleton<ShaderManager>
    {
    private:
        /// @brief The map of shaders and their users(programs) count
        std::unordered_set<std::unique_ptr<BaseShader>, unsigned int> shaders_;

    public:
    };
}

#endif // MSCE_SHADER_MANAGER_H_