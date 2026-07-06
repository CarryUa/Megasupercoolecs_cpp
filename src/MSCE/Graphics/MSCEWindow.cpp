#include "MSCEWindow.h"
#include <MSCE/BuiltIns/transformComponent.hpp>
#include <MSCE/Managers/prototypeManager.h>
#include <MSCE/Managers/systemManager.h>
#include <MSCE/Graphics/color.h>
#include <MSCE/Managers/eventManager.h>
#include <MSCE/Events/glEvents.h>
#include <MSCE/ECS/entity.h>
#include <MSCE/BuiltIns/RenderComponents/baseRendererComponent.hpp>
#include <iostream>
#include <memory>
using namespace std;
using namespace msce;

namespace
{
    void transform_vertecies(float *dest, const VertexList &in_vert, Vertex center, TransformComponent *comp, MSCEWindow &window)
    {

        size_t offset = 0;

        for (auto v : in_vert)
        {

            v.x -= comp->shape.get()->get_w() / 2;
            v.y -= comp->shape.get()->get_h() / 2;

            v /= window.size();

            v += comp->position / 2;

            v *= comp->scale;

            dest[offset++] = static_cast<float>(v.x);
            dest[offset++] = static_cast<float>(v.y);
            dest[offset++] = 0.0;
        }
    }
}

// void msce::MSCEWindow::on_all_prototypes_ready(msce::PrototypeLoadingFinishedEvent &ev)
// {
//     this->make_curent_context();

//     auto prototypes = PrototypeManager::instance->enumerate_prototypes<ShaderPrototype>();

//     for (const auto *sp : prototypes) // load all
//     {
//         if (!sp)
//             continue;

//         const ShaderPrototype &sp_ref = *sp;

//         logger.log_debug("Preparing shader '{}'...", sp->id);
//         this->context_.shaders.push_back(std::make_shared<IShader>(sp_ref));
//         logger.log_debug("Done", sp->id);
//     }
// }

// MSCEWindow::MSCEWindow(Vector2D<int> window_size_, const char *title, GLFWmonitor *glfw_monitor, GLFWwindow *glfw_share) : p_window_(glfwCreateWindow(window_size_.x, window_size_.y, title, glfw_monitor, glfw_share), &glfwDestroyWindow)
// {
//     this->comp_man_ = ComponentManager::instance;
//     this->t_sys_ = SystemManager::instance->get_system<TimeSystem>();

//     this->window_size = Vector2D(window_size_);
//     this->title = title;

//     if (p_window_.get() == nullptr)
//     {
//         glfwTerminate();

//         throw runtime_error("Failed to create GLFW window");
//     }

//     glfwMakeContextCurrent(p_window_.get());

//     vao_ = vbo_ = 0;

//     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//     {
//         glfwTerminate();
//         throw runtime_error("Failed to initialize GLAD");
//     }

//     if (!PrototypeManager::instance)
//         MSCE_SUBSCRIBE_TO_EVENT_NON_STATIC(msce::PrototypeLoadingFinishedEvent, this->on_all_prototypes_ready);
//     else
//     {
//         PrototypeLoadingFinishedEvent ev;
//         on_all_prototypes_ready(ev);
//     }

//     glfwSetFramebufferSizeCallback(this->p_window_.get(), this->on_resize);

//     glGenVertexArrays(1, &vao_);
//     glGenBuffers(1, &vbo_);
// }

// void msce::MSCEWindow::on_resize(GLFWwindow *window, int w, int h)
// {
//     glfwMakeContextCurrent(window);
//     glViewport(0, 0, w, h);
// }

// void MSCEWindow::make_curent_context()
// {
//     glfwMakeContextCurrent(p_window_.get());
// }

// bool MSCEWindow::should_close()
// {
//     return glfwWindowShouldClose(this->p_window_.get());
// }

// void msce::MSCEWindow::render()
// {
//     this->make_curent_context();
//     glClear(GL_COLOR_BUFFER_BIT);
//     glClearColor(0, 0, 0, 1);

//     auto transforms = comp_man_->get_all_components_of_type<TransformComponent>();
//     double time = t_sys_->get_total_seconds();
//     double sin_t_norm_r = (sin(time) + 1) / 2;
//     double sin_t_norm_g = (sin(time + M_PI / 1.5) + 1) / 2;
//     double sin_t_norm_b = (sin(time - M_PI / 1.5) + 1) / 2;
//     for (auto t : transforms)
//     {
//         Color fg(vec3uc(static_cast<unsigned char>(sin_t_norm_r * 255),
//                         static_cast<unsigned char>(sin_t_norm_g * 255),
//                         static_cast<unsigned char>(sin_t_norm_b * 255)));

//         VertexList vertices_vec = t->shape.get()->get_vertecies();
//         size_t buffer_s = vertices_vec.size() * 3;
//         auto vertices = make_unique<float[]>(buffer_s);
//         transform_vertecies(vertices.get(), vertices_vec, t->position, t.get(), (*this));

//         // Bind the VAO first
//         glBindVertexArray(vao_);

//         // Bind VBO and buffer the vertex data
//         glBindBuffer(GL_ARRAY_BUFFER, vbo_);
//         glBufferData(GL_ARRAY_BUFFER, buffer_s * sizeof(float), vertices.get(), GL_STATIC_DRAW);

//         // Configure vertex attribute pointers (position at layout location 0)
//         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
//         glEnableVertexAttribArray(0);

//         // Unbind to prevent accidental modifications
//         glBindBuffer(GL_ARRAY_BUFFER, 0);
//         glBindVertexArray(0);

//         glUseProgram(current_program_->get_handle());
//         glBindVertexArray(vao_);
//         auto color_uniform = glGetUniformLocation(current_program_->get_handle(), "color");
//         glUniform4f(color_uniform, (static_cast<float>(fg.r()) / 255.0), (static_cast<float>(fg.g()) / 255.0), (static_cast<float>(fg.b()) / 255.0), (static_cast<float>(fg.a()) / 255.0));

//         glDrawArrays(GL_TRIANGLE_FAN, 0, buffer_s / 3);
//     }
// }

// void MSCEWindow::swap_buffers()
// {
//     glfwSwapBuffers(this->p_window_.get());
// }

void msce::MSCEWindow::prepare_shaders()
{

    if (!PrototypeManager::instance)
        MSCE_SUBSCRIBE_TO_EVENT_NON_STATIC(PrototypeLoadingFinishedEvent, prepare_shaders);

    else
    {
        int suc = 0, fail = 0;
        this->use_context();

        logger_.log_debug("Preparing shaders...");
        auto shader_prototypes = PrototypeManager::instance->enumerate_prototypes<ShaderPrototype>();
        for (const auto *sp : shader_prototypes)
        {
            if (!sp)
                continue;

            auto shrd = std::make_shared<Shader>(*sp);

            if (shrd->is_ready())
                suc++;
            else
                fail++;

            this->context_.shaders.insert_or_assign(sp->id, std::move(shrd));
            logger_.log_debug("Loaded '{}'", sp->id);
        }
        logger_.log_debug("Finished compiling shaders. total: {} | successes: {} | failures: {}", suc + fail, suc, fail);
    }
}

void msce::MSCEWindow::use_context()
{
    glfwMakeContextCurrent(p_window_.get());
}

msce::MSCEWindow::MSCEWindow(vec2ui window_size, const char *title, GLFWmonitor *glfw_monitor, GLFWwindow *glfw_share) : p_window_(glfwCreateWindow(window_size.x, window_size.y, title, glfw_monitor, glfw_share), &glfwDestroyWindow),
                                                                                                                         size_(window_size),
                                                                                                                         title_(title),
                                                                                                                         logger_(std::format("Window('{}')", title))
{
    if (!p_window_)
    {
        logger_.log_error("Failed to create window '{}'.", title_);
        throw std::runtime_error("Failed to create glfw window.");
    }

    this->use_context();

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        logger_.log_error("Failed to load glad GL-loader for this window.");
        throw std::runtime_error("Failed to load GL-loader.");
    }

    logger_.log_info("GL version: {}", reinterpret_cast<const char *>(glGetString(GL_VERSION)));

    glGenBuffers(1, &this->context_.vbo);
    glGenVertexArrays(1, &this->context_.vao);

    prepare_shaders();
}

vec2ui msce::MSCEWindow::size()
{
    return size_;
}

void msce::MSCEWindow::size(vec2ui s)
{
    size_ = s;
}

bool msce::MSCEWindow::should_close()
{
    return glfwWindowShouldClose(this->p_window_.get());
}

void msce::MSCEWindow::render()
{
    this->use_context();

    glfwPollEvents();

    RenderStartEvent ev(*this);
    EventManager::instance->fire(ev);

    for (const auto &transform : ComponentManager::instance->get_all_components_of_type<TransformComponent>())
    {
        if (!transform)
            continue;
        auto ent = transform->get_entity();

        if (!ent)
            continue;

        auto renderer = ent->get_component<BaseRendererComponent>();

        if (!renderer)
            continue;

        if (renderer->shader_id.empty())
            continue;

        if (!context_.shaders.contains(renderer->shader_id))
            continue;
        if (!context_.shaders[renderer->shader_id]->is_ready())
            continue;

        auto prog = this->context_.shaders[renderer->shader_id]->get_handle();

        auto verts = transform->shape->get_vertecies();

        auto transformed_verts = std::make_unique<float[]>(verts.size() * 3);

        transform_vertecies(transformed_verts.get(), verts, vec2d(0, 0), transform.get(), *this);

        glBindVertexArray(this->context_.vao);

        glBindBuffer(GL_ARRAY_BUFFER, this->context_.vbo);
        glBufferData(GL_ARRAY_BUFFER, verts.size() * 3 * sizeof(float), transformed_verts.get(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glUseProgram(prog);
        glBindVertexArray(this->context_.vao);

        auto color_uniform = glGetUniformLocation(prog, "color");
        glUniform4f(color_uniform, renderer->color.r() / 255.0f, renderer->color.g() / 255.0f, renderer->color.b() / 255.0f, renderer->color.a() / 255.0f);

        glDrawArrays(GL_TRIANGLE_FAN, 0, verts.size());
    }
}

void msce::MSCEWindow::draw()
{
    this->use_context();
    glfwSwapBuffers(this->p_window_.get());
}
