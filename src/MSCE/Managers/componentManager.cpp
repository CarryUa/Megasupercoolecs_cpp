#include "componentManager.h"
#include <iostream>
#include <MSCE/ECS/component.h>

using namespace msce;

void msce::ComponentManager::update_smallest_available_id()
{

    if (this->smallest_available_id_ - _components.size() - 1 > this->min_distance_to_reuse_)
    {
        this->_components.push_back(nullptr);
        this->smallest_available_id_++;
        return;
    }

    for (size_t i = this->_components.size() - 1; i > 0; --i)
        if (this->_components[i] == nullptr)
        {
            this->smallest_available_id_ = i;
            return;
        }

    this->_components.resize(this->_components.size() + 1);
    this->smallest_available_id_ = this->_components.size() - 1;
}

msce::ComponentManager::ComponentManager()
{
    this->_components.push_back(nullptr);
}

void msce::ComponentManager::destroy_component(IComponent *comp)
{
    this->destroy_component(comp->id_);
}

void msce::ComponentManager::destroy_component(size_t id)
{
    if (id > this->_components.size())
        return;

    this->_components[id].reset();
    this->smallest_available_id_ = id;

    alive_components_count_--;
}

std::size_t msce::ComponentManager::count() const noexcept
{
    return this->alive_components_count_;
}
