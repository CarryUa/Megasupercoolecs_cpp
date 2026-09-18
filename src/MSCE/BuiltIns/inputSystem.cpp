#include "inputSystem.h"

#include <MSCE/Managers/eventManager.h>

using namespace msce;

namespace
{
InputAction determain_action(int action)
{
  switch (action)
  {
  case GLFW_PRESS:
    return InputAction::PRESSED;
    break;
  case GLFW_REPEAT:
    return InputAction::HELD;
    break;
  case GLFW_RELEASE:
    return InputAction::RELEASED;
    break;
  };

  return InputAction::INVALID;
}
} // namespace

void msce::InputSystem::on_glfw_key_input(GLFWwindow *, int key, int scancode,
                                          int action, int mods)
{
  KeyEventArgs args{.key = {.keycode = key, .scancode = scancode, .mods = mods},
                    .action = determain_action(action)};

  InputSystem::on_keyboard_input.fire(args);
}

void msce::InputSystem::on_glfw_cursor_moved(GLFWwindow *, double xpos,
                                             double ypos)
{
  CursorMoveEventArgs args;

  args.pos = cursor_pos = vec2d(xpos, ypos);
  InputSystem::on_cursor_move.fire(args);
}

void msce::InputSystem::on_glfw_mouse_btn(GLFWwindow *window, int button,
                                          int action, int mods)
{
  MouseButtonEventArgs event{.pos = InputSystem::get_cursor_position(window),
                             .mods = mods,
                             .action = determain_action(action)};

  switch (button)
  {
  case GLFW_MOUSE_BUTTON_LEFT:
    event.button = MouseButton::LEFT_MB;
    break;

  case GLFW_MOUSE_BUTTON_RIGHT:
    event.button = MouseButton::RIGHT_MB;
    break;

  case GLFW_MOUSE_BUTTON_MIDDLE:
    event.button = MouseButton::MIDDLE_MB;
    break;
  }
  on_mouse_input.fire(event);
}

void msce::InputSystem::on_window_created(WindowCreatedEvent &ev)
{
  glfwSetKeyCallback(ev.window.get_glfw_window(), on_glfw_key_input);
  glfwSetCursorPosCallback(ev.window.get_glfw_window(), on_glfw_cursor_moved);
  glfwSetMouseButtonCallback(ev.window.get_glfw_window(), on_glfw_mouse_btn);
}

void msce::InputSystem::init()
{
  MSCE_SUBSCRIBE_TO_EVENT(WindowCreatedEvent, on_window_created);
}

vec2d msce::InputSystem::get_cursor_position(GLFWwindow *window)
{
  vec2d pos;
  glfwGetCursorPos(window, &pos.x, &pos.y);
  return pos;
}

vec2d msce::InputSystem::get_cursor_position(MSCEWindow &window)
{
  return get_cursor_position(window.get_glfw_window());
}

bool msce::InputKey::shift_held() const noexcept
{
  return mods & GLFW_MOD_SHIFT;
}

bool msce::InputKey::control_held() const noexcept
{
  return mods & GLFW_MOD_CONTROL;
}

bool msce::InputKey::alt_held() const noexcept { return mods & GLFW_MOD_ALT; }

bool msce::InputKey::super_held() const noexcept
{
  return mods & GLFW_MOD_SUPER;
}

bool msce::InputKey::caps_lock_on() const noexcept
{
  return mods & GLFW_MOD_CAPS_LOCK;
}

bool msce::InputKey::num_lock_on() const noexcept
{
  return mods & GLFW_MOD_NUM_LOCK;
}

bool msce::MouseButtonEventArgs::shift_held() const noexcept
{
  return mods & GLFW_MOD_SHIFT;
}

bool msce::MouseButtonEventArgs::control_held() const noexcept
{
  return mods & GLFW_MOD_CONTROL;
}

bool msce::MouseButtonEventArgs::alt_held() const noexcept
{
  return mods & GLFW_MOD_ALT;
}

bool msce::MouseButtonEventArgs::super_held() const noexcept
{
  return mods & GLFW_MOD_SUPER;
}

bool msce::MouseButtonEventArgs::caps_lock_on() const noexcept
{
  return mods & GLFW_MOD_CAPS_LOCK;
}

bool msce::MouseButtonEventArgs::num_lock_on() const noexcept
{
  return mods & GLFW_MOD_NUM_LOCK;
}
