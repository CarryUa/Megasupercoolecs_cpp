#include "MSCEWindow.h"
#include <MSCE/BuiltInComponents/transformComponent.hpp>
#include <MSCE/Managers/systemManager.h>
#include <iostream>
#include <memory>
using namespace std;
using namespace msce;

const string vertexShaderSource = "#version 330 core\n"
                                  "layout (location = 0) in vec3 aPos;\n"
                                  "void main()\n"
                                  "{\n"
                                  "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                  "}\0";

const string fragmentShaderSource = "#version 330 core\n"
                                    "out vec4 FragColor;\n"
                                    "void main()\n"
                                    "{\n"
                                    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n" // Orange color
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

MSCEWindow::MSCEWindow(Vector2D<int> window_size_, const char *title, GLFWmonitor *glfw_monitor, GLFWwindow *glfw_share) : p_window(glfwCreateWindow(window_size_.x, window_size_.y, title, glfw_monitor, glfw_share), &glfwDestroyWindow)
{
    static Logger l("Window");
    this->compMan_ = ComponentManager::instance;
    this->tSys_ = SystemManager::instance->get_system<TimeSystem>();

    this->window_size = Vector2D(window_size_);
    this->title = title;

    if (p_window.get() == nullptr)
    {
        glfwTerminate();

        throw runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(p_window.get());

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        throw runtime_error("Failed to initialize GLAD");
    }

    const char *vShaderCode = vertexShaderSource.c_str();
    const char *fShaderCode = fragmentShaderSource.c_str();

    GLuint vertexSh_, fragSh_;
    int success;
    char infoLog[512];

    // Vertex Shader
    vertexSh_ = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexSh_, 1, &vShaderCode, NULL);
    glCompileShader(vertexSh_);
    // Check for compilation errors
    glGetShaderiv(vertexSh_, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexSh_, 512, NULL, infoLog);
        l.log_error("Failed to compile vertex shader: {}", infoLog);
    }

    // Fragment Shader
    fragSh_ = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragSh_, 1, &fShaderCode, NULL);
    glCompileShader(fragSh_);
    // Check for compilation errors
    glGetShaderiv(fragSh_, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragSh_, 512, NULL, infoLog);
        l.log_error("Failed to compile fragment shader: {}", infoLog);
    }

    // Shader Program
    this->shaderProgram_ = glCreateProgram();
    glAttachShader(this->shaderProgram_, vertexSh_);
    glAttachShader(this->shaderProgram_, fragSh_);
    glLinkProgram(this->shaderProgram_);
    // Check for linking errors
    glGetProgramiv(this->shaderProgram_, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(this->shaderProgram_, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(vertexSh_);
    glDeleteShader(fragSh_);

    glfwSetFramebufferSizeCallback(this->p_window.get(), this->on_resize);
}

void msce::MSCEWindow::on_resize(GLFWwindow *window, int w, int h)
{
    glfwMakeContextCurrent(window);
    glViewport(0, 0, w, h);
}

void MSCEWindow::make_curent_context()
{
    glfwMakeContextCurrent(p_window.get());
}

bool MSCEWindow::should_close()
{
    return glfwWindowShouldClose(this->p_window.get());
}

void msce::MSCEWindow::render()
{
    static float x = 0, y = 1;

    x = -tanf(tSys_->get_delta_time() / 2);
    y = sinf(tSys_->get_delta_time() * 2);
    this->make_curent_context();
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0, 0, 0, 1);

    auto transforms = compMan_->get_all_components_of_type<TransformComponent>();
    // 1. Define the vertices for a triangle

    for (auto t : transforms)
    {
        VertexList vertices_vec = t->shape.get()->get_vertecies();
        size_t buffer_s = vertices_vec.size() * 3;
        auto vertices = make_unique<float[]>(buffer_s);
        transform_vertecies(vertices.get(), vertices_vec, t->position, t, (*this));

        unsigned int VBO, VAO;
        // Generate 1 VAO and 1 VBO
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        // Bind the VAO first
        glBindVertexArray(VAO);

        // Bind VBO and buffer the vertex data
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, buffer_s * sizeof(float), vertices.get(), GL_STATIC_DRAW);

        // Configure vertex attribute pointers (position at layout location 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // Unbind to prevent accidental modifications
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glUseProgram(shaderProgram_);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, buffer_s / 3);
    }
}

void MSCEWindow::swap_buffers()
{
    glfwSwapBuffers(this->p_window.get());
}