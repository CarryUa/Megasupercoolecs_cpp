#include "component.h"

using namespace msce;

std::size_t Component::get_id() const
{
    return this->_id;
}
Component::Component(size_t id)
{
    this->_id = id;
}