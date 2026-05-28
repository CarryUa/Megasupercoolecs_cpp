#include "enumManager.h"

msce::EnumManager::EnumManager() : registered_enum_factories_ref_(get_g_enum_factories_registry())
{
}

msce::Enum msce::EnumManager::create_enum(const std::string &enum_name, int64_t value)
{
    if (!this->registered_enum_factories_ref_.is_registered(enum_name))
        logger.log_error("Tried to create enum with unregistered name '{}'. Check for typos!", enum_name);

    auto e = this->registered_enum_factories_ref_.get_entry(enum_name)();
    e.set(value);
    return e;
}

msce::Enum msce::EnumManager::create_enum(const std::string &enum_name, const std::string &value)
{
    if (!this->registered_enum_factories_ref_.is_registered(enum_name))
        logger.log_error("Tried to create enum with unregistered name '{}'. Check for typos!", enum_name);

    auto e = this->registered_enum_factories_ref_.get_entry(enum_name)();
    e.set(value);
    return e;
}
