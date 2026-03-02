#include "graphicsSystem.h"
#include <iostream>

using namespace msce;
using namespace std;

void GraphicsSystem::init()
{
    if (!glfwInit())
    {
        throw std::runtime_error("Couldn't initialize glfw.");
    }
}

void GraphicsSystem::select_window(size_t id)
{
    MSCEWindow *window = this->get_window(id);
    if (window == nullptr)
        cerr << "Window (" << id << ") wasn't selected." << endl;

    window->make_curent_context();
}

void GraphicsSystem::select_window(MSCEWindow *window)
{
    window->make_curent_context();
}

MSCEWindow *GraphicsSystem::get_window(size_t id)
{
    if (this->_windows.size() <= id)
    {
        cerr << "Can't find window at (" << id << "), thera are only " << this->_windows.size() << " windows availible" << endl;
        return nullptr;
    }

    return this->_windows[id].get();
}

MSCEWindow *GraphicsSystem::create_window(Vector2D<int> window_size, const char *title, GLFWmonitor *glfw_monitor, GLFWwindow *glfw_share)
{
    this->_windows.push_back(make_unique<MSCEWindow>(window_size, title, glfw_monitor, glfw_share));

    return this->_windows.back().get();
}