#pragma once
#include <MSCE/system.h>
#include "graphicsSystem.h"
#include <MSCE/Events/graphicsEvents.h>

namespace msce
{
enum class InputAction
{
  INVALID = -1,
  PRESSED,
  HELD,
  RELEASED
};
enum class MouseButton
{
  LEFT_MB = GLFW_MOUSE_BUTTON_LEFT,
  RIGHT_MB = GLFW_MOUSE_BUTTON_RIGHT,
  MIDDLE_MB = GLFW_MOUSE_BUTTON_MIDDLE
};

struct InputKey
{
  int keycode;
  int scancode;
  int mods;

  bool shift_held() const noexcept;
  bool control_held() const noexcept;
  bool alt_held() const noexcept;
  bool super_held() const noexcept;
  bool caps_lock_on() const noexcept;
  bool num_lock_on() const noexcept;
};

struct KeyEventArgs
{
  InputKey key;
  InputAction action;
};

struct CursorMoveEventArgs
{
  vec2d pos;
};

struct MouseButtonEventArgs
{
  vec2d pos;
  MouseButton button;
  int mods;
  InputAction action;

  bool shift_held() const noexcept;
  bool control_held() const noexcept;
  bool alt_held() const noexcept;
  bool super_held() const noexcept;
  bool caps_lock_on() const noexcept;
  bool num_lock_on() const noexcept;
};

/**
 * @brief Handles basic input stuff, like keyboard and mouse.
 */
class InputSystem : public System
{
  inline static vec2d cursor_pos = vec2d(0, 0);

  static void on_glfw_key_input(GLFWwindow *, int key, int scancode, int action,
                                int mods);

  static void on_glfw_cursor_moved(GLFWwindow *, double xpos, double ypos);
  static void on_glfw_mouse_btn(GLFWwindow *window, int button, int action,
                                int mods);

  static void on_window_created(WindowCreatedEvent &ev);

  virtual void init() override;

public:
  /**
   * @returns Position of the cursor on given window.
   */
  static vec2d get_cursor_position(MSCEWindow &window);
  /**
   * @returns Position of the cursor on given window.
   */
  static vec2d get_cursor_position(GLFWwindow *window);

  /**
   * @brief Fired when any keyboard key is pressed/held/released.
   */
  inline static LocalEvent<KeyEventArgs> on_keyboard_input;
  /**
   * @brief Fired when cursor moves.
   */
  inline static LocalEvent<CursorMoveEventArgs> on_cursor_move;
  /**
   * @brief Fired when either of the main 3 mouse buttons is
   * pressed/held/released.
   */
  inline static LocalEvent<MouseButtonEventArgs> on_mouse_input;
};

} // namespace msce
MSCE_REGISTER_SYSTEM(msce::InputSystem)