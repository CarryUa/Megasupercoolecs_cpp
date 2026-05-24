#include "graphicsSystem.h"
#include <MSCE/BuiltIns/transformComponent.hpp>
#include <glad/glad.h>
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

msce::GraphicsSystem::GraphicsSystem()
{
    this->comp_man_ = ComponentManager::instance;
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

void msce::GraphicsSystem::pre_rended_all_objects_on_window(MSCEWindow *window)
{
}

MSCEWindow *GraphicsSystem::get_window(size_t id)
{
    if (this->windows_.size() <= id)
    {
        cerr << "Can't find window at (" << id << "), thera are only " << this->windows_.size() << " windows availible" << endl;
        return nullptr;
    }

    return this->windows_[id].get();
}

MSCEWindow *GraphicsSystem::create_window(Vector2D<int> window_size, const char *title, GLFWmonitor *glfw_monitor, GLFWwindow *glfw_share)
{
    this->windows_.push_back(make_unique<MSCEWindow>(window_size, title, glfw_monitor, glfw_share));

    return this->windows_.back().get();
}