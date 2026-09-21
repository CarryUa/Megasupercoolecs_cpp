#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <memory>

#include <MSCE/Types/vector.h>
#include <MSCE/BuiltIns/timeSystem.h>
#include <MSCE/shader.h>
#include <MSCE/Events/prototypeEvents.h>
#include <MSCE/Types/image.h>

using namespace std;

namespace msce
{
class MSCEWindow
{
private:
  struct GLContext
  {
    std::unordered_map<std::string, std::shared_ptr<Shader>> shaders = {};
    std::unordered_map<const Image *, GLuint> textures;
    GLuint vao = 0, vbo = 0, ebo = 0;
  } context_;

  const Logger logger_;
  const std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> p_window_;
  std::string title_;
  vec2ui size_;

  void prepare_shaders();

  void use_context();

public:
  MSCEWindow(vec2ui window_size_ = vec2ui(1600, 900),
             const char *title = "msce", GLFWmonitor *glfw_monitor = nullptr,
             GLFWwindow *glfw_share = nullptr);
  vec2ui size();

  std::shared_ptr<Shader> &get_shader_ref(const std::string &shader_id);

  GLFWwindow *get_glfw_window() const noexcept;

  /**
   * @brief Generates a texture for an image for future use. Binding happens
   * automaticaly when trying to draw an image.
   * @returns True if image bount successfuly, false otherwise
   */
  bool bind_image(const Image &image);
  void unbind_image(const Image &image);

  /// @return Whether image is currently bound
  bool image_bound(const Image &image) const;

  GLuint get_texture(const Image &image) const;
  bool try_get_texture(const Image &image, GLuint &out_handle) const noexcept;

  void size(vec2ui s);

  bool should_close();

  void render();

  void draw();

  void reload_shaders();
};
} // namespace msce
