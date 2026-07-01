#include "enum.hpp"
using namespace msce;

Registry<std::string, std::function<Enum()>> &msce::get_g_enum_factories_registry()
{
    static Registry<std::string, std::function<Enum()>> reg;
    return reg;
}

std::vector<std::reference_wrapper<const std::type_info>> &msce::get_g_enum_types_vec()
{
    static std::vector<std::reference_wrapper<const std::type_info>> vec;
    return vec;
}