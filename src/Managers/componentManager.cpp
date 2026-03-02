#include "componentManager.h"
using namespace msce;

template <typename TComp>
TComp *ComponentManager::get_component(size_t id) const
{
    if (_components.size() <= id)
        return nullptr;

    return _components[id].get();
}

template <typename TComp>
std::vector<TComp *> ComponentManager::get_components() const
{
    std::vector<TComp *> return_vec;

    for (auto &comp : this->_components)
    {
        return_vec.push_back(&comp);
    }

    return return_vec;
}
