#pragma once

#include <MSCE/MSCEWindow.h>
#include <MSCE/system.h>
#include <MSCE/component.h>
#include <MSCE/Types/vector.h>
#include <vector>
#include <memory>
class GLFWmonitor;
class GLFWwindow;
namespace msce
{

/// @brief Handles GL-related stuff. Uses several sub-systems like ShaderSystem,
/// ect.
class GraphicsSystem : public System
{
private:
  std::vector<std::unique_ptr<MSCEWindow>> windows_;
  ComponentManager *comp_man_ = nullptr;

public:
  GraphicsSystem();
  MSCEWindow *get_window(size_t id);
  MSCEWindow *create_window(Vector2D<int> window_size_ = Vector2D<int>(500,
                                                                       500),
                            const char *title = "Window",
                            GLFWmonitor *glfw_monitor = nullptr,
                            GLFWwindow *glfw_share = nullptr);
  // void destroy_window(MSCEWindow *window);
  void init() override;
};
} // namespace msce
MSCE_REGISTER_SYSTEM(msce::GraphicsSystem)
