#include "MSCEWindow.h"
#include <MSCE/BuiltIns/transformComponent.hpp>
#include <MSCE/Managers/systemManager.h>
#include <MSCE/Graphics/color.h>
#include <iostream>
#include <memory>
using namespace std;
using namespace msce;

const string vertex_shader_source = "#version 330 core\n"
                                    "layout (location = 0) in vec3 aPos;\n"
                                    "void main()\n"
                                    "{\n"
                                    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                    "}\0";

const string fragment_shader_source = "#version 330 core\n"
                                      "uniform vec4 color;\n"
                                      "out vec4 FragColor;\n"
                                      "void main()\n"
                                      "{\n"
                                      "   FragColor = (color / 255);\n"
                                      "}\n\0";

static void transform_vertecies(float *dest, const VertexList &in_vert, Vertex center, TransformComponent *comp, MSCEWindow &window)
{

    size_t offset = 0;

    for (auto v : in_vert)
    {

        v.x -= comp->shape.get()->get_w() / 2;
        v.y -= comp->shape.get()->get_h() / 2;

        v /= window.window_size;

        v += comp->position / 2;

        v *= comp->scale;

        dest[offset++] = static_cast<float>(v.x);
        dest[offset++] = static_cast<float>(v.y);
        dest[offset++] = 0.0;
    }
}

MSCEWindow::MSCEWindow(Vector2D<int> window_size_, const char *title, GLFWmonitor *glfw_monitor, GLFWwindow *glfw_share) : p_window_(glfwCreateWindow(window_size_.x, window_size_.y, title, glfw_monitor, glfw_share), &glfwDestroyWindow)
{
    static Logger l("Window");
    this->comp_man_ = ComponentManager::instance;
    this->t_sys_ = SystemManager::instance->get_system<TimeSystem>();

    this->window_size = Vector2D(window_size_);
    this->title = title;

    if (p_window_.get() == nullptr)
    {
        glfwTerminate();

        throw runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(p_window_.get());

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        throw runtime_error("Failed to initialize GLAD");
    }

    const char *v_shader_code = vertex_shader_source.c_str();
    const char *f_shader_code = fragment_shader_source.c_str();

    GLuint vertex_sh, frag_sh;
    int success;
    char info_log[512];

    // Vertex Shader
    vertex_sh = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_sh, 1, &v_shader_code, NULL);
    glCompileShader(vertex_sh);
    // Check for compilation errors
    glGetShaderiv(vertex_sh, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_sh, 512, NULL, info_log);
        l.log_error("Failed to compile vertex shader: {}", info_log);
    }

    // Fragment Shader
    frag_sh = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_sh, 1, &f_shader_code, NULL);
    glCompileShader(frag_sh);
    // Check for compilation errors
    glGetShaderiv(frag_sh, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(frag_sh, 512, NULL, info_log);
        l.log_error("Failed to compile fragment shader: {}", info_log);
    }

    // Shader Program
    this->shader_program_ = glCreateProgram();
    glAttachShader(this->shader_program_, vertex_sh);
    glAttachShader(this->shader_program_, frag_sh);
    glLinkProgram(this->shader_program_);
    // Check for linking errors
    glGetProgramiv(this->shader_program_, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(this->shader_program_, 512, NULL, info_log);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << info_log << std::endl;
    }

    glDeleteShader(vertex_sh);
    glDeleteShader(frag_sh);

    glfwSetFramebufferSizeCallback(this->p_window_.get(), this->on_resize);
}

void msce::MSCEWindow::on_resize(GLFWwindow *window, int w, int h)
{
    glfwMakeContextCurrent(window);
    glViewport(0, 0, w, h);
}

void MSCEWindow::make_curent_context()
{
    glfwMakeContextCurrent(p_window_.get());
}

bool MSCEWindow::should_close()
{
    return glfwWindowShouldClose(this->p_window_.get());
}

void msce::MSCEWindow::render()
{

    this->make_curent_context();
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0, 0, 0, 1);

    auto transforms = comp_man_->get_all_components_of_type<TransformComponent>();
    for (auto t : transforms)
    {
        Color fg(vec3uc((sin(t_sys_->get_total_seconds()) / 2 + 1) * 255,
                        (sin(t_sys_->get_total_seconds() + M_PI / 1.5) / 2 + 1) * 255,
                        (sin(t_sys_->get_total_seconds() - M_PI / 1.5) / 2 + 1) * 255));

        VertexList vertices_vec = t->shape.get()->get_vertecies();
        size_t buffer_s = vertices_vec.size() * 3;
        auto vertices = make_unique<float[]>(buffer_s);
        transform_vertecies(vertices.get(), vertices_vec, t->position, t.get(), (*this));

        unsigned int vbo, vao;
        // Generate 1 VAO and 1 VBO
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        // Bind the VAO first
        glBindVertexArray(vao);

        // Bind VBO and buffer the vertex data
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, buffer_s * sizeof(float), vertices.get(), GL_STATIC_DRAW);

        // Configure vertex attribute pointers (position at layout location 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // Unbind to prevent accidental modifications
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glUseProgram(shader_program_);
        glBindVertexArray(vao);
        auto color_uniform = glGetUniformLocation(shader_program_, "color");
        glUniform4f(color_uniform, fg.r(), fg.g(), fg.b(), fg.a());

        glDrawArrays(GL_TRIANGLE_FAN, 0, buffer_s / 3);
    }
}

void MSCEWindow::swap_buffers()
{
    glfwSwapBuffers(this->p_window_.get());
}