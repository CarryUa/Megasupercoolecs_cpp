#include "reflection.h"
#include <MSCE/msce_macros.h>
#include <cstring>

namespace
{
    constexpr uint16_t trait_category_mask = 0b0000'0000'0000'0111;
    constexpr uint16_t trait_spec_mask = 0b0000'0000'0011'1111;
    constexpr uint16_t trait_shared_mask = 0b1111'1111'1100'0000;

}

const std::type_info &msce::Type::get_std_type() const
{
    return this->std_type_;
}

const char *msce::Type::get_name() const
{
    return this->name_;
}

std::string msce::Type::get_name_str() const
{
    return std::string(this->name_);
}

bool msce::Type::has_member(const MemberInfo &member) const
{
    for (const auto &m : this->members_)
    {
        if (member == m)
            return true;
    }

    return false;
}

bool msce::Type::has_member_named(const char *name) const
{
    for (const auto &m : this->members_)
    {
        if (std::strcmp(m.get_name(), name) == 0)
            return true;
    }
    return false;
}

const msce::MemberInfo &msce::Type::get_member_named(const char *name) const
{

    for (const auto &m : this->members_)
    {
        if (std::strcmp(m.get_name(), name) == 0)
            return m;
    }

    logger.log_error("Tried to get member '{}' from '{}', but said object doesn't have said member!", name, this->name_);
    throw std::runtime_error("Reflection get_member_named() failed.");
}

uint32_t msce::Type::get_size() const
{
    return this->size_;
}

std::vector<std::reference_wrapper<const msce::MemberInfo>> msce::Type::get_members() const
{
    std::vector<std::reference_wrapper<const msce::MemberInfo>> r;

    for (const auto &m : this->members_)
    {
        r.push_back(std::cref(m));
    }
    return r;
}

bool msce::Type::operator==(const Type &other) const
{
    return this->std_type_ == other.std_type_;
}

bool msce::MemberInfo::operator==(const MemberInfo &other) const
{
    return this->type_info_ == other.type_info_;
}

uint32_t msce::MemberInfo::get_offset() const
{
    return this->offset_;
}

std::string msce::MemberInfo::get_name_str() const
{
    return std::string(this->name_);
}

const msce::Type &msce::MemberInfo::get_type() const
{
    return this->type_info_;
}

const msce::Type &msce::get_reflection_of_type(const std::string &name)
{
    try
    {
        return msce::internal::get_g_reflection_types_registry().get_entry(name);
    }
    catch (std::out_of_range &e)
    {
        static Logger l("Reflection(get type function)");
        l.log_error("Tried to get reflection info of type '{}', but it wasn't registered with that name. Make sure there's no mismatch between macros. (Watch out for :: key)", name);
        throw std::runtime_error("Reflection function msce::get_reflection_of_type(const std::string&) defined at '" + MSCE_FILE_TRACK_STR() + "' failed for type " + name);
    }
}

const msce::Type &msce::get_reflection_of_type(const std::type_info &std_type)
{
    for (const auto &[n, t] : internal::get_g_reflection_types_registry().enumerate_registry())
    {
        if (t.get().get_std_type() == std_type)
            return t;
    }
    throw std::runtime_error("Reflection function msce::get_reflection_of_type(const std::type_info&) defined at '" + MSCE_FILE_TRACK_STR() + "' failed for type " + std::string(std_type.name()));
}
