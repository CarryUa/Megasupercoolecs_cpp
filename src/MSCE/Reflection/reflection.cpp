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

bool msce::Type::is_numeric() const
{
    return (this->traits_ & trait_category_mask) == (uint16_t)TypeTraitFlags::NUMERIC;
}

bool msce::Type::is_integer() const
{
    return (this->traits_ & (uint16_t)TypeTraitFlags::INTEGRAL) == (uint16_t)TypeTraitFlags::INTEGRAL;
}

bool msce::Type::is_floating_point() const
{
    return (this->traits_ & (uint16_t)TypeTraitFlags::FLOATING_POINT) == (uint16_t)TypeTraitFlags::FLOATING_POINT;
}

bool msce::Type::is_unsigned() const
{
    return (this->traits_ & (uint16_t)TypeTraitFlags::UNSIGNED) == (uint16_t)TypeTraitFlags::UNSIGNED;
}

bool msce::Type::is_class() const
{
    return (this->traits_ & trait_category_mask) == (uint16_t)TypeTraitFlags::CLASS;
}

bool msce::Type::is_pointer() const
{
    return (this->traits_ & trait_category_mask) == (uint16_t)TypeTraitFlags::POINTER;
}

bool msce::Type::is_member_object_pointer() const
{
    return (this->traits_ & (uint16_t)TypeTraitFlags::MEMBER_OBJECT_POINTER) == (uint16_t)TypeTraitFlags::MEMBER_OBJECT_POINTER;
}

bool msce::Type::is_reference() const
{
    return (this->traits_ & trait_category_mask) == (uint16_t)TypeTraitFlags::REFERENCE;
}

bool msce::Type::is_lvalue_reference() const
{
    return (this->traits_ & (uint16_t)TypeTraitFlags::LVALUE) == (uint16_t)TypeTraitFlags::LVALUE;
}

bool msce::Type::is_rvalue_reference() const
{
    return (this->traits_ & (uint16_t)TypeTraitFlags::RVALUE) == (uint16_t)TypeTraitFlags::RVALUE;
}

bool msce::Type::is_array() const
{
    return (this->traits_ & trait_category_mask) == (uint16_t)TypeTraitFlags::ARRAY;
}

bool msce::Type::is_bounded_array() const
{
    return (this->traits_ & (uint16_t)TypeTraitFlags::BOUNDED) == (uint16_t)TypeTraitFlags::BOUNDED;
}

bool msce::Type::is_enum() const
{
    return (this->traits_ & trait_category_mask) == (uint16_t)TypeTraitFlags::ENUM;
}

bool msce::Type::is_function() const
{
    return (this->traits_ & trait_category_mask) == (uint16_t)TypeTraitFlags::FUNCTION;
}

bool msce::Type::is_void() const
{
    return (this->traits_ & trait_category_mask) == (uint16_t)TypeTraitFlags::VOID;
}

bool msce::Type::is_const() const
{
    return (this->traits_ & trait_shared_mask) == (uint16_t)TypeTraitFlags::CONST;
}

bool msce::Type::is_volatile() const
{
    return (this->traits_ & trait_shared_mask) == (uint16_t)TypeTraitFlags::VOLATILE;
}

bool msce::Type::is_default_constructible() const
{
    return (this->traits_ & trait_shared_mask) == (uint16_t)TypeTraitFlags::DEFAULT_CONSTRUCTABLE;
}

bool msce::Type::is_copy_constructible() const
{
    return (this->traits_ & trait_shared_mask) == (uint16_t)TypeTraitFlags::COPY_CONSTRUCTABLE;
}

bool msce::Type::is_move_constructible() const
{
    return (this->traits_ & trait_shared_mask) == (uint16_t)TypeTraitFlags::MOVE_CONSTRUCTABLE;
}

bool msce::Type::is_copy_asignable() const
{
    return (this->traits_ & trait_shared_mask) == (uint16_t)TypeTraitFlags::COPY_ASIGNABLE;
}

bool msce::Type::is_move_asignable() const
{
    return (this->traits_ & trait_shared_mask) == (uint16_t)TypeTraitFlags::MOVE_ASIGNABLE;
}

bool msce::Type::operator==(const Type &other) const
{
    return this->std_type_ == other.std_type_;
}

msce::MemberInfo::MemberInfo(const char *name, uint32_t offset, const Type &type) : name_(name), offset_(offset), type_info_(type)
{
    logger.log_debug("Created member '{}' with offset({}) and type '{}'", name, offset_, type.get_name());
}

bool msce::MemberInfo::operator==(const MemberInfo &other) const
{
    return this->type_info_ == other.type_info_;
}

uint32_t msce::MemberInfo::get_offset() const
{
    return this->offset_;
}

constexpr const char *msce::MemberInfo::get_name() const
{
    return this->name_;
}

std::string msce::MemberInfo::get_name_str() const
{
    return std::string(this->name_);
}

const msce::Type &msce::get_reflection_of_type(const std::string &name)
{
    try
    {
        return msce::internal::get_g_reflection_types_registry().get_entry(name);
    }
    catch (std::out_of_range &e)
    {
        static Logger l("Reflection()");
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
