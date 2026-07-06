// #ifndef MSCE_SHADER_MANAGER_H_
// #define MSCE_SHADER_MANAGER_H_

// #include <MSCE/Types/singleton.hpp>
// #include <MSCE/Prototypes/shaderPrototype.hpp>
// #include <MSCE/Events/prototypeEvents.h>
// #include <MSCE/logger.h>

// namespace msce
// {

//     class ShaderManager : public Singleton<ShaderManager>
//     {
//     private:
//         inline static msce::Logger logger = msce::Logger("ShaderManager");
//         std::vector<std::shared_ptr<IShader>> shaders_;

//         void on_all_prototypes_ready(msce::PrototypeLoadingFinishedEvent &ev);

//     public:
//         ShaderManager();

//         template <typename T>
//         std::shared_ptr<T> get_shader();
//     };

// }

// #endif // MSCE_SHADER_MANAGER_H_