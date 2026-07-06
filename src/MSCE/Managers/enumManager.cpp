#include "enumManager.h"

msce::EnumManager::EnumManager() : registered_enum_factories_ref_(get_g_enum_factories_registry()), registered_enum_types_ref_(get_g_enum_types_vec())
{
    logger.log_info("Initialized EnumManager!");
}

msce::Enum msce::EnumManager::create_enum(const std::string &enum_name, int64_t value)
{
    if (!this->registered_enum_factories_ref_.is_registered(enum_name))
    {
        logger.log_error("Tried to create enum with unregistered name '{}'. Check for typos!", enum_name);
        throw std::runtime_error("Failed to create enum.");
    }

    auto e = this->registered_enum_factories_ref_.get_entry(enum_name)();
    e.set(value);
    return e;
}

bool msce::EnumManager::is_type_enum(const std::type_info &t) const
{
    for (const auto &reg_t : this->registered_enum_types_ref_)
    {
        if (reg_t.get() == t)
            return true;
    }
    return false;
}

bool msce::EnumManager::enum_exists(const std::string &enum_name) const
{
    return this->registered_enum_factories_ref_.is_registered(enum_name);
}

msce::Enum msce::EnumManager::create_enum(const std::string &enum_name, const std::string &value)
{
    if (!this->registered_enum_factories_ref_.is_registered(enum_name))
        logger.log_error("Tried to create enum with unregistered name '{}'. Check for typos!", enum_name);

    auto e = this->registered_enum_factories_ref_.get_entry(enum_name)();
    e.set(value);
    return e;
}
