#include "componentManager.h"
#include <iostream>
#include <MSCE/ECS/component.h>

using namespace msce;

void msce::ComponentManager::register_component(IComponent *comp)
{
    for (auto &c : this->_components)
    {
        if (c.get() == comp)
            return;
    }

    this->_components.push_back(std::unique_ptr<IComponent>(comp));
}

void msce::ComponentManager::destroy_component(IComponent *comp)
{
    bool found = false;
    size_t i = 0;
    for (auto &c : this->_components)
    {
        if (c.get() == comp)
        {
            found = true;
            break;
        }

        ++i;
    }

    if (!found)
        return;

    this->_components.erase(_components.begin() + i);
}

size_t msce::ComponentManager::get_component_id(IComponent *comp)
{
    size_t i = 0;
    for (; i < this->_components.size(); ++i)
    {
        if (this->_components[i].get() == comp)
            break;
    }

    return i;
}
