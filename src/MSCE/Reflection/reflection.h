#ifndef MSCE_REFLECTION_H_
#define MSCE_REFLECTION_H_
#include "reflection_concepts.h"

#include <span>
#include <cstdint>
#include <typeindex>

#include <MSCE/logger.h>
#include <MSCE/Types/enum.hpp>
#include <MSCE/Types/Collections/registry.hpp>

#include <boost/preprocessor.hpp>

namespace msce
{
    class Type;
    class MemberInfo;

    /**
     * Relevant documentation can be found at @ref msce::Type::traits_
     */
    enum class TypeTraitFlags : uint16_t
    {
        NUMERIC = 0,
        CLASS = 1,
        POINTER = 2,
        REFERENCE = 3,
        ARRAY = 4,
        ENUM = 5,
        FUNCTION = 6,
        VOID = 7,

        /* NUMERIC */
        INTEGRAL = NUMERIC | 1 << 3,
        FLOATING_POINT = NUMERIC | 1 << 4,
        UNSIGNED = NUMERIC | 1 << 5,

        /* CLASS */

        /* POINTER */
        MEMBER_OBJECT_POINTER = POINTER | 1 << 4,

        /* REFERENCE */
        LVALUE = REFERENCE | 1 << 3,
        RVALUE = REFERENCE | 1 << 4,

        /* ARRAY */
        BOUNDED = ARRAY | 1 << 3,

        /* FUNCTION */

        /* VOID */

        /* SHARED */
        CONST = 1 << 6,
        VOLATILE = 1 << 7,

        DEFAULT_CONSTRUCTABLE = 1 << 9,
        COPY_CONSTRUCTABLE = 1 << 10,
        MOVE_CONSTRUCTABLE = 1 << 11,
        COPY_ASSIGNABLE = 1 << 12,
        MOVE_ASSIGNABLE = 1 << 13,
    };

    namespace internal
    {
        template <typename T>
        struct TypeRegistration
        {
            inline static constexpr const msce::Type &get_type()
            {
                static_assert(false, "Type is never registered in reflection. Try using MSCE_REFLECT_CLASS or MSCE_REFLECT_FUNDAMENTAL to fix the issue.");
                throw std::runtime_error("Tried to get a type reflection that is not reflected at point its reflection was requested.");
            }
        };

        /**
         * @brief Internal function used by reflection.
         * @tparam T The type to compute traits of.
         * @returns Returns a bitmap of @ref msce::TypeTraitFlags of given type.
         */
        template <typename T>
        inline constexpr uint16_t compute_type_traits()
        {
            uint16_t flags = 0;
            if constexpr (std::is_arithmetic_v<T>)
            {
                if constexpr (std::is_integral_v<T>)
                {
                    flags |= static_cast<uint16_t>(TypeTraitFlags::INTEGRAL);
                }
                else
                {
                    flags |= static_cast<uint16_t>(TypeTraitFlags::FLOATING_POINT);
                }

                if constexpr (!std::is_signed_v<T>)
                {
                    flags |= static_cast<uint16_t>(TypeTraitFlags::UNSIGNED);
                }
            }
            else if constexpr (std::is_enum_v<T>)
            {
                flags |= static_cast<uint16_t>(TypeTraitFlags::ENUM);
            }
            else if constexpr (std::is_class_v<T>)
            {
                flags |= static_cast<uint16_t>(TypeTraitFlags::CLASS);
            }
            else if constexpr (std::is_invocable_v<T>)
            {
                flags |= static_cast<uint16_t>(TypeTraitFlags::FUNCTION);
            }
            // Important to notice here, that I explicitly exclude function and member function pointers from POINTER category.
            // They fall into FUNCTION category, since I use std::is_invocable_v to check for functions.
            else if constexpr (std::is_pointer_v<T>)
            {
                flags |= static_cast<uint16_t>(TypeTraitFlags::POINTER);
                if constexpr (std::is_member_object_pointer_v<T>)
                    flags |= static_cast<uint16_t>(TypeTraitFlags::MEMBER_OBJECT_POINTER);
            }
            else if constexpr (std::is_reference_v<T>)
            {
                flags |= static_cast<uint16_t>(TypeTraitFlags::REFERENCE);
                if constexpr (std::is_lvalue_reference_v<T>)
                    flags |= static_cast<uint16_t>(TypeTraitFlags::LVALUE);
                else
                    flags |= static_cast<uint16_t>(TypeTraitFlags::RVALUE);
            }
            else if constexpr (std::is_array_v<T>)
            {
                flags |= static_cast<uint16_t>(TypeTraitFlags::ARRAY);
                if constexpr (std::is_bounded_array_v<T>)
                    flags |= static_cast<uint16_t>(TypeTraitFlags::BOUNDED);
            }

            else
                flags |= static_cast<uint16_t>(TypeTraitFlags::VOID);

            /* SHARED stuff */
            if constexpr (std::is_const_v<T>)
                flags |= static_cast<uint16_t>(TypeTraitFlags::CONST);
            if constexpr (std::is_volatile_v<T>)
                flags |= static_cast<uint16_t>(TypeTraitFlags::VOLATILE);
            if constexpr (std::is_default_constructible_v<T>)
                flags |= static_cast<uint16_t>(TypeTraitFlags::DEFAULT_CONSTRUCTABLE);
            if constexpr (std::is_copy_constructible_v<T>)
                flags |= static_cast<uint16_t>(TypeTraitFlags::COPY_CONSTRUCTABLE);
            if constexpr (std::is_move_constructible_v<T>)
                flags |= static_cast<uint16_t>(TypeTraitFlags::MOVE_CONSTRUCTABLE);
            if constexpr (std::is_move_assignable_v<T>)
                flags |= static_cast<uint16_t>(TypeTraitFlags::MOVE_ASSIGNABLE);
            if constexpr (std::is_copy_assignable_v<T>)
                flags |= static_cast<uint16_t>(TypeTraitFlags::COPY_ASSIGNABLE);

            return flags;
        }

        inline Registry<std::string, std::reference_wrapper<const Type>> &get_g_reflection_types_registry()
        {
            static Registry<std::string, std::reference_wrapper<const Type>> reg;
            return reg;
        }

        /**
         * @brief Internal function used by reflection. Calculates the offset of member pointer passed in.
         * @tparam TC Type of compound containint member. Dublicate it into Context tparam.
         * @tparam TM Type of member under the pointer.
         * @tparam Context The actual type. You need to pass it it to avoid template deduction.
         */
        // template <typename Context, typename TC, typename TM>
        // inline constexpr uint32_t get_offset_of_member(TM TC::*m_ptr)
        // {
        //     static_assert(ClassWithReflection<Context>,
        //                   "Context must satisfy the ClassWithReflection concept!");

        //     // const Context *c = nullptr;
        //     // const TM *m = &(c->*m_ptr);

        //     return __builtin_offsetof(Context, m_ptr);

        //     // return static_cast<uint32_t>(
        //     //     reinterpret_cast<const char *>(m) - reinterpret_cast<const char *>(c));
        // }
    }

    /**
     * @brief Class-representation of individual reflected member.
     */
    class MemberInfo
    {
    private:
        inline static Logger logger = Logger("Reflection(MemberInfo)");

        /// @brief Reference to @ref msce::Type of this member.
        const Type &type_info_;
        /// @brief Human-readable name of this member.
        const char *name_;
        /// @brief The offset from start of the container object to this member.
        const uint32_t offset_;

    public:
        constexpr MemberInfo(const char *name, uint32_t offset, const Type &type);

        /**
         * @param object Reference to target object.
         * @returns Value of member represented by this in given object if present.
         * @throws std::runtime_error if given object doesn't contain this member.
         */
        template <typename TValue, ClassWithReflection TObject>
        TValue get_value(const TObject &object) const;

        template <typename TValue = void>
        TValue get_value(const void *object, const Type &o_type) const;
        /**
         * @brief Sets the value of member represented by this in given object if present.
         * @param object Reference to target object.
         * @param value Honestly I dont know what this could be :)
         * @throws std::runtime_error if given object doesn't contain this member.
         */
        template <typename TValue, ClassWithReflection TObject>
        void set_value(const TObject &object, TValue value) const;

        template <typename TValue = void>
        void set_value(const void *object, TValue value, const Type &o_type) const;

        /**
         * @retval Returns type-casted pointer to the member represented by this in given object if present.
         * @retval nullptr otherwise.
         */
        template <typename TValue, ClassWithReflection TObject>
        TValue *get_ptr(const TObject &object) const;

        template <typename TValue = void>
        TValue *get_ptr(const void *object, const Type &o_type) const;

        template <ClassWithReflection TObject>
        /**
         * @retval Returns untyped(void) pointer to the member represented by this in given object if present.
         * @retval nullptr otherwise.
         */
        void *get_ptr(const TObject &object) const;

        bool operator==(const MemberInfo &other) const;

        /**
         * @brief Getter for this member's offset
         */
        uint32_t get_offset() const;

        /**
         * @brief Getter for this member's name.
         */
        constexpr const char *get_name() const;
        /**
         * @brief Getter for this member's stringified name
         */
        std::string get_name_str() const;

        const Type &get_type() const;
    };

    /**
     * @brief Class-representation of reflected type.
     */
    class Type
    {
    private:
        inline static const Logger logger = Logger("Reflection(Type)");

        /// @brief std::type_info reference of underlying type.
        const std::type_info &std_type_;
        /// @brief Human-readable name of this type.
        const char *name_;
        /// @brief The size of this type.
        const uint64_t size_;
        /**
         * @brief These are flags containing this type's traits. You can find flags at @ref msce::TypeTraitFlags.
         * @details This is not conventional bitmap. Bits 0 trough 2 are most significant. They define mutually-exclusive type "category", like class, numeric, enum, etc.
         * @details Bits 3 trough 5 are category-exclusive specifications. (eg. INGTEGRAL flag is exclusive to NUMERIC category).
         * @details Remaining bits are shader between all categories. (eg. any type can be constant).
         * @note 1) Note that lack of UNSIGNED flag is treated as number being signed.
         * @note 2) Note that lack of BOUNDED flag is treated as array being unbounded.
         * @warning Checking the flags requires special mask(see @ref msce::Type is_*() functions implementation for more info), therefore it is not recomended to use them directly.
         */
        const uint16_t traits_;
        /// @brief The members of this type.
        const std::span<const MemberInfo> members_;

    public:
        constexpr Type(const char *name, uint64_t size, uint16_t trait_flags, std::span<const MemberInfo> members, const std::type_info &std_type) : name_(name),
                                                                                                                                                     size_(size),
                                                                                                                                                     traits_(trait_flags),
                                                                                                                                                     members_(members),
                                                                                                                                                     std_type_(std_type)

        {
        }

        /**
         * @returns Constant std::type_info reference to type represented by this.
         */
        const std::type_info &get_std_type() const;

        /**
         * @brief Getter for this member's name.
         */
        const char *get_name() const;
        /**
         * @brief Getter for this member's stringified name.
         */
        std::string get_name_str() const;

        /**
         * @retval True if this type has provided member.
         * @retval False otherwise.
         */
        bool has_member(const MemberInfo &member) const;
        /**
         * @retval True if this type has member with given name.
         * @retval False otherwise.
         */
        bool has_member_named(const char *name) const;
        /**
         * @returns Constant @ref msce::MemberInfo reference representation of member with given name.
         * @throws std::runtime_error if member with given name does not exist in this type.
         */
        const MemberInfo &get_member_named(const char *name) const;

        /**
         * @returns Value of member with given name in given object.
         * @throws std::runtime_error if member with given name does not exist in given object.
         */
        template <typename TValue, ClassWithReflection TObject>
        TValue get_member_value(const TObject &object, const char *member_name) const;

        template <typename TValue>
        TValue get_member_value(const void *object, const char *member_name) const;

        /**
         * @brief Sets the value of member with given name in given object to provided one.
         * @throws std::runtime_error if member with given name does not exist in given object.
         */
        template <typename TValue, ClassWithReflection TObject>
        void set_member_value(const TObject &object, const char *member_name, TValue value) const;

        template <typename TValue>
        void set_member_value(const void *object, const char *member_name, const Type &o_type, TValue value) const;

        template <typename TValue>
        TValue get_value(std::remove_pointer_t<std::remove_reference_t<TValue>> &target) const;

        template <typename TValue>
        TValue get_value(const void *target) const;

        template <typename TValue>
        void set_value(std::remove_pointer_t<std::remove_reference_t<TValue>> &target, TValue value) const;

        template <typename TValue>
        void set_value(void *target, TValue value) const;

        /**
         * @brief Getter for size of this object. Same as sizeof(RepresentedType).
         */
        uint32_t get_size() const;

        std::vector<std::reference_wrapper<const MemberInfo>> get_members() const;

        /* I am absolutely NOT documenting 24 self-explainatory methods. Go back to pre-school if you can't figure it out.*/

        constexpr bool is_numeric() const;
        constexpr bool is_integer() const;
        constexpr bool is_floating_point() const;
        constexpr bool is_unsigned() const;

        constexpr bool is_class() const;

        constexpr bool is_pointer() const;
        constexpr bool is_member_object_pointer() const;

        constexpr bool is_reference() const;
        constexpr bool is_lvalue_reference() const;
        constexpr bool is_rvalue_reference() const;

        constexpr bool is_array() const;
        constexpr bool is_bounded_array() const;

        constexpr bool is_enum() const;
        constexpr bool is_function() const;
        constexpr bool is_void() const;

        constexpr bool is_const() const;
        constexpr bool is_volatile() const;

        constexpr bool is_default_constructible() const;
        constexpr bool is_copy_constructible() const;
        constexpr bool is_move_constructible() const;

        constexpr bool is_copy_assignable() const;
        constexpr bool is_move_assignable() const;

        bool operator==(const Type &other) const;
    };

    template <typename T>
    constexpr const Type &get_reflection_of_type()
    {
        return internal::TypeRegistration<T>::get_type();
    }
    const Type &get_reflection_of_type(const std::string &name);
    const Type &get_reflection_of_type(const std::type_info &std_type);

#define typeof(Type) ::msce::get_reflection_of_type<Type>()
#define typeof_v(Variable) ::msce::get_reflection_of_type(typeid(Variable))
#define typeof_n(Str) ::msce::get_reflection_of_type(Str)

    inline constexpr msce::MemberInfo::MemberInfo(const char *name, uint32_t offset, const Type &type) : name_(name), offset_(offset), type_info_(type)
    {
    }

    template <typename TValue, ClassWithReflection TObject>
    inline TValue MemberInfo::get_value(const TObject &object) const
    {
        return this->get_value<TValue>(reinterpret_cast<const void *>(&object), typeof(TObject));
    }
    template <typename TValue>
    inline TValue MemberInfo::get_value(const void *object, const Type &o_type) const
    {
        if (!object)
        {
            logger.log_error("Tried to get value of null object.");
            throw std::runtime_error("Invalid reflection operation.");
        }
        if (!o_type.has_member((*this)))
        {
            logger.log_error("Tried to get value of '{}' in '{}', but it has no such member.", this->get_name(), o_type.get_name());
            throw std::runtime_error("Invalid reflection operation.");
        }

        auto m_addr = reinterpret_cast<uintptr_t>(this->get_ptr(object, o_type));

        if constexpr (typeof(TValue).is_integer())
        {
            if (this->get_type().is_enum())
            {
                switch (this->get_type().get_size())
                {
                case 1:
                {
                    if (this->get_type().is_unsigned())
                        return *reinterpret_cast<uint8_t *>(m_addr);
                    else
                        return *reinterpret_cast<int8_t *>(m_addr);
                }
                case 2:
                {
                    if (this->get_type().is_unsigned())
                        return *reinterpret_cast<uint16_t *>(m_addr);
                    else
                        return *reinterpret_cast<int16_t *>(m_addr);
                }
                case 4:
                {

                    if (this->get_type().is_unsigned())
                        return *reinterpret_cast<uint32_t *>(m_addr);
                    else
                        return *reinterpret_cast<int32_t *>(m_addr);
                }
                case 8:
                {
                    if (this->get_type().is_unsigned())
                        return *reinterpret_cast<uint64_t *>(m_addr);
                    else
                        return *reinterpret_cast<int64_t *>(m_addr);
                }
                }
                logger.log_warning("Invalid integer size '{}'. Returning default...", this->get_type().get_size());
                return TValue();
            }
        }

        return *reinterpret_cast<TValue *>(m_addr);
    }
    template <typename TValue, ClassWithReflection TObject>
    inline void MemberInfo::set_value(const TObject &object, TValue value) const
    {
        this->set_value(reinterpret_cast<const void *>(&object), value, typeof(TObject));

        // if (!TObject::get_type_info().has_member(*this))
        //     throw std::runtime_error("Tried to set value of member '" + this->get_name_str() + "' in '" + TObject::get_type_info().get_name_str() + "', but said object doesn't have said member.");

        // auto m = this->get_ptr<TValue>(object);
        // if (!m)
        // {
        //     logger.log_error("Tried to set value of member '{}' in object {}, but said object has no such member", this->get_name_str(), TObject::get_type_info().get_name_str());
        //     return;
        // }
        // *m = value;
    }
    template <typename TValue>
    inline void MemberInfo::set_value(const void *object, TValue value, const Type &o_type) const
    {
        if (!o_type.has_member(*this))
            throw std::runtime_error(std::format("Tried to set value of member '{}' in '{}', but said object doesn't have said member.", this->get_name(), o_type.get_name()));

        if constexpr (typeof(TValue).is_integer())
        {
            if (this->get_type().is_enum())
            {
                void *addr = this->get_ptr(object, o_type);
                switch (this->get_type().get_size())
                {
                case 1:
                {
                    if (this->get_type().is_unsigned())
                        *reinterpret_cast<uint8_t *>(addr) = static_cast<uint8_t>(value);
                    else
                        *reinterpret_cast<int8_t *>(addr) = static_cast<int8_t>(value);
                    return;
                }
                case 2:
                {
                    if (this->get_type().is_unsigned())
                        *reinterpret_cast<uint16_t *>(addr) = static_cast<uint16_t>(value);
                    else
                        *reinterpret_cast<int16_t *>(addr) = static_cast<int16_t>(value);
                    return;
                }
                case 4:
                {

                    if (this->get_type().is_unsigned())
                        *reinterpret_cast<uint32_t *>(addr) = static_cast<uint32_t>(value);
                    else
                        *reinterpret_cast<int32_t *>(addr) = static_cast<int32_t>(value);

                    return;
                }
                case 8:
                {
                    if (this->get_type().is_unsigned())
                        *reinterpret_cast<uint64_t *>(addr) = static_cast<uint64_t>(value);
                    else
                        *reinterpret_cast<int64_t *>(addr) = static_cast<int64_t>(value);
                    return;
                }
                }
            }
        }
        // else
        // {
        auto m = this->get_ptr<TValue>(object, o_type);
        if (!m)
        {
            logger.log_error("Tried to set value of member '{}' in object {}, but said object has no such member", this->get_name(), o_type.get_name());
            return;
        }
        *m = value;
        // }
    }
    template <typename TValue, ClassWithReflection TObject>
    inline TValue *MemberInfo::get_ptr(const TObject &object) const
    {

        if constexpr (std::is_integral_v<TValue> || std::is_enum_v<TValue>)
            if (this->type_info_.is_enum() && this->type_info_.get_size() == sizeof(TValue))
            {
                return reinterpret_cast<TValue *>(this->get_ptr(object));
            }

        if (this->type_info_.get_std_type() != typeid(TValue))
        {
            logger.log_error("Tried to set value of member '{}' in object {}, but said object has no such member", this->get_name_str(), TObject::get_type_info().get_name_str());
            return nullptr;
        }

        return reinterpret_cast<TValue *>(this->get_ptr(object));
    }
    template <typename TValue>
    inline TValue *MemberInfo::get_ptr(const void *object, const Type &o_type) const
    {
        if (!object)
            return nullptr;

        if (!o_type.has_member(*this))
        {
            logger.log_error("Tried to get pointer-to member '{}' in object '{}', but no such member in this object exists.", o_type.get_name(), this->get_name());
            return nullptr;
        }

        uintptr_t o_addr = reinterpret_cast<uintptr_t>(object);
        uintptr_t m_addr = o_addr + this->offset_;

        return reinterpret_cast<TValue *>(m_addr);
    }
    template <ClassWithReflection TObject>
    inline void *MemberInfo::get_ptr(const TObject &object) const
    {

        if (!TObject::get_type_info().has_member(*this))
        {
            logger.log_error("Tried to set value of member '{}' in object {}, but said object has no such member", this->get_name_str(), TObject::get_type_info().get_name_str());
            return nullptr;
        }
        if (this->offset_ > TObject::get_type_info().get_size())
            return nullptr;

        auto m_addr = reinterpret_cast<uintptr_t>(&object) + this->offset_;
        return reinterpret_cast<void *>(m_addr);
    }

    inline constexpr const char *msce::MemberInfo::get_name() const
    {
        return this->name_;
    }

    template <typename TValue, ClassWithReflection TObject>
    inline TValue Type::get_member_value(const TObject &object, const char *member_name) const
    {
        const auto &m = this->get_member_named(member_name);
        return m.get_value<TValue>(object);
    }

    template <typename TValue>
    inline TValue Type::get_member_value(const void *object, const char *member_name) const
    {
        const auto &m = this->get_member_named(member_name);
        return m.get_value<TValue>(object, *this);
    }

    template <typename TValue, ClassWithReflection TObject>
    inline void Type::set_member_value(const TObject &object, const char *member_name, TValue value) const
    {
        this->set_member_value<TValue>(reinterpret_cast<const void *>(&object), member_name, *this, value);
    }

    template <typename TValue>
    inline void Type::set_member_value(const void *object, const char *member_name, const Type &o_type, TValue value) const
    {
        const auto &m = this->get_member_named(member_name);
        m.set_value(object, value, o_type);
    }

    template <typename TValue>
    inline TValue Type::get_value(std::remove_pointer_t<std::remove_reference_t<TValue>> &target) const
    {
        using RawTValue = std::remove_pointer_t<std::remove_reference_t<TValue>>;
        void *v_addr = reinterpret_cast<void *>(&target);

        if constexpr (std::is_pointer_v<TValue>)
        {
            return reinterpret_cast<TValue>(v_addr);
        }
        else if constexpr (std::is_reference_v<TValue>)
        {
            return *reinterpret_cast<RawTValue *>(v_addr);
        }
        else
        {
            if constexpr (!typeof(RawTValue).is_copy_constructible())
            {
                logger.log_error("Tried to get copy-of-value of type '{}', but it is not copy-constructible", this->get_name_str());
            }

            return *reinterpret_cast<RawTValue *>(v_addr);
        }
    }

    template <typename TValue>
    inline TValue Type::get_value(const void *target) const
    {
        static_assert(msce::TypeWithReflection<TValue>, "TValue must be reflected.");
        using RawT = std::remove_pointer_t<std::remove_reference_t<TValue>>;

        const Type &t = msce::get_reflection_of_type<RawT>();
        if (t != *this)
        {
            logger.log_error("Tried to get value of type '{}' from object of type '{}'", t.get_name(), this->get_name());
            throw std::runtime_error("Type mismatch. See logs for details.");
        }

        if (!target)
        {
            if constexpr (get_reflection_of_type<TValue>().is_pointer())
            {
                return nullptr;
            }

            logger.log_error("Tried to get value of nullptr object of expected type '{}'", this->get_name());
            throw std::runtime_error("Failed to get value of null object trough reflection");
        }

        const TValue *v_ptr = reinterpret_cast<const TValue *>(target);

        if constexpr (get_reflection_of_type<TValue>().is_pointer())
        {
            return v_ptr;
        }
        else
        {
            if constexpr (get_reflection_of_type<TValue>().is_reference() || get_reflection_of_type<TValue>().is_copy_constructible())
            {
                return *v_ptr;
            }

            else
            {
                logger.log_error("Tried to return copy of non-copy-constructible type '{}'", this->get_name());
                throw std::runtime_error("Invalid copy. See logs for details");
            }
        }

        return *v_ptr;
    }

    template <typename TValue>
    inline void Type::set_value(std::remove_pointer_t<std::remove_reference_t<TValue>> &target, TValue value) const
    {
        using RawTValue = std::remove_pointer_t<std::remove_reference_t<TValue>>;
        if (get_reflection_of_type<TValue>() != *this)
            return;

        void *v_ptr = reinterpret_cast<void *>(&target);

        if constexpr (get_reflection_of_type<TValue>().is_reference())
        {
            logger.log_error("Failed to set value of reference type '{}'. You can not reassign references after initialization.", this->get_name_str());
            return;
        }

        if constexpr (get_reflection_of_type<TValue>().is_pointer())
        {
            *reinterpret_cast<TValue *>(v_ptr) = value;
        }
        else
        {
            if constexpr (get_reflection_of_type<TValue>().is_copy_assignable())
            {
                *reinterpret_cast<RawTValue *>(v_ptr) = value;
            }
        }
    }

    template <typename TValue>
    inline void Type::set_value(void *target, TValue value) const
    {
        using RawType = std::remove_cv_t<std::remove_reference_t<std::remove_pointer_t<TValue>>>;
        if constexpr (get_reflection_of_type<TValue>().is_reference())
        {
            logger.log_error("Tried to set reference value post-initializarion for type '{}'", this->get_name());
            return;
        }

        if (this->is_const())
        {
            logger.log_error("Tried to set value of constant post-initializarion for type '{}'", this->get_name());
            return;
        }

        if (typeid(value) != this->get_std_type())
            return;

        RawType *v_ptr = reinterpret_cast<RawType *>(target);

        if constexpr (get_reflection_of_type<TValue>().is_pointer())
        {
            v_ptr = value;
        }
        else
        {
            if constexpr (get_reflection_of_type<TValue>().is_copy_constructible())
            {
                *v_ptr = value;
                return;
            }

            logger.log_error("Tried to set value of non-copy-constructible type '{}' by copy.", this->get_name());
        }
    }

    inline constexpr bool msce::Type::is_numeric() const
    {
        return (this->traits_ & 0x7) == (uint16_t)TypeTraitFlags::NUMERIC;
    }

    inline constexpr bool msce::Type::is_integer() const
    {
        return (this->traits_ & (uint16_t)TypeTraitFlags::INTEGRAL) == (uint16_t)TypeTraitFlags::INTEGRAL;
    }

    inline constexpr bool msce::Type::is_floating_point() const
    {
        return (this->traits_ & (uint16_t)TypeTraitFlags::FLOATING_POINT) == (uint16_t)TypeTraitFlags::FLOATING_POINT;
    }

    inline constexpr bool msce::Type::is_unsigned() const
    {
        return (this->traits_ & (uint16_t)TypeTraitFlags::UNSIGNED) == (uint16_t)TypeTraitFlags::UNSIGNED;
    }

    inline constexpr bool msce::Type::is_class() const
    {
        return (this->traits_ & 0x7) == (uint16_t)TypeTraitFlags::CLASS;
    }

    inline constexpr bool msce::Type::is_pointer() const
    {
        return (this->traits_ & 0x7) == (uint16_t)TypeTraitFlags::POINTER;
    }

    inline constexpr bool msce::Type::is_member_object_pointer() const
    {
        return (this->traits_ & (uint16_t)TypeTraitFlags::MEMBER_OBJECT_POINTER) == (uint16_t)TypeTraitFlags::MEMBER_OBJECT_POINTER;
    }

    inline constexpr bool msce::Type::is_reference() const
    {
        return (this->traits_ & 0x7) == (uint16_t)TypeTraitFlags::REFERENCE;
    }

    inline constexpr bool msce::Type::is_lvalue_reference() const
    {
        return (this->traits_ & (uint16_t)TypeTraitFlags::LVALUE) == (uint16_t)TypeTraitFlags::LVALUE;
    }

    inline constexpr bool msce::Type::is_rvalue_reference() const
    {
        return (this->traits_ & (uint16_t)TypeTraitFlags::RVALUE) == (uint16_t)TypeTraitFlags::RVALUE;
    }

    inline constexpr bool msce::Type::is_array() const
    {
        return (this->traits_ & 0x7) == (uint16_t)TypeTraitFlags::ARRAY;
    }

    inline constexpr bool msce::Type::is_bounded_array() const
    {
        return (this->traits_ & (uint16_t)TypeTraitFlags::BOUNDED) == (uint16_t)TypeTraitFlags::BOUNDED;
    }

    inline constexpr bool msce::Type::is_enum() const
    {
        return (this->traits_ & 0x7) == (uint16_t)TypeTraitFlags::ENUM;
    }

    inline constexpr bool msce::Type::is_function() const
    {
        return (this->traits_ & 0x7) == (uint16_t)TypeTraitFlags::FUNCTION;
    }

    inline constexpr bool msce::Type::is_void() const
    {
        return (this->traits_ & 0x7) == (uint16_t)TypeTraitFlags::VOID;
    }

    inline constexpr bool msce::Type::is_const() const
    {
        return (this->traits_ & (uint16_t)TypeTraitFlags::CONST) != 0;
    }

    inline constexpr bool msce::Type::is_volatile() const
    {
        return (this->traits_ & (uint16_t)TypeTraitFlags::VOLATILE) != 0;
    }

    inline constexpr bool msce::Type::is_default_constructible() const
    {
        return (this->traits_ & (uint16_t)TypeTraitFlags::DEFAULT_CONSTRUCTABLE) != 0;
    }

    inline constexpr bool msce::Type::is_copy_constructible() const
    {
        return (this->traits_ & (uint16_t)TypeTraitFlags::COPY_CONSTRUCTABLE) != 0;
    }

    inline constexpr bool msce::Type::is_move_constructible() const
    {
        return (this->traits_ & (uint16_t)TypeTraitFlags::MOVE_CONSTRUCTABLE) != 0;
    }

    inline constexpr bool msce::Type::is_copy_assignable() const
    {
        return (this->traits_ & (uint16_t)TypeTraitFlags::COPY_ASSIGNABLE) != 0;
    }

    inline constexpr bool msce::Type::is_move_assignable() const
    {
        return (this->traits_ & (uint16_t)TypeTraitFlags::MOVE_ASSIGNABLE) != 0;
    }
}

/**
 * @brief Simply generates static get_type_info() method ad adds internal functions as friends. You don't HAVE to define it, but it's highly advised.
 * @brief You do need to add it to reflect non-public members tho.
 */
#define MSCE_REFLECTION_DEFINE_CLASS(TypeName)                                    \
    template <typename T>                                                         \
    friend constexpr const ::msce::Type &get_reflection_of_type();                \
    inline static const ::msce::Type &get_type_info()                             \
    {                                                                             \
        static const ::msce::Type &t = ::msce::get_reflection_of_type(#TypeName); \
        return t;                                                                 \
    }

#define MSCE_FUNDAMENTAL_HELPER(TypeName, UID)                                                                      \
    namespace msce::internal                                                                                        \
    {                                                                                                               \
        template <>                                                                                                 \
        struct TypeRegistration<TypeName>                                                                           \
        {                                                                                                           \
            inline static constexpr msce::Type type = msce::Type(#TypeName,                                         \
                                                                 sizeof(TypeName),                                  \
                                                                 ::msce::internal::compute_type_traits<TypeName>(), \
                                                                 {},                                                \
                                                                 typeid(TypeName));                                 \
            inline static constexpr const msce::Type &get_type()                                                    \
            {                                                                                                       \
                return type;                                                                                        \
            }                                                                                                       \
            static void register_self()                                                                             \
            {                                                                                                       \
                static const ::msce::Type &r = []() {                 Logger logger("StaticTypeRegistration");                                                                                        \
                ::msce::internal::get_g_reflection_types_registry().register_entry(#TypeName, ::std::cref(TypeRegistration<TypeName>::type)); \
                logger.log_info("Successfully reflected type '{}'", TypeRegistration<TypeName>::type.get_name_str());                          \
                    return TypeRegistration<TypeName>::type; }();                                                          \
            }                                                                                                       \
            [[gnu::used]] TypeRegistration()                                                                        \
            {                                                                                                       \
                register_self();                                                                                    \
            }                                                                                                       \
        };                                                                                                          \
        inline static TypeRegistration<TypeName> BOOST_PP_CAT(refl_t_reg, UID);                                     \
    }

#define MSCE_REFLECT_CLASS_MEMBER_IMPL(ClassName, MemberName) \
    ::msce::MemberInfo(#MemberName, __builtin_offsetof(ClassName, MemberName), ::msce::get_reflection_of_type<decltype(ClassName::MemberName)>())
#define MSCE_REFLECT_CLASS_MEMBER_WRP(r, data, x) \
    MSCE_REFLECT_CLASS_MEMBER_IMPL(data, x)

#define MSCE_REFLECT_CLASS_HELPER(ClassName, UID, ...)                                                                                       \
    namespace msce::internal                                                                                                                 \
    {                                                                                                                                        \
        template <>                                                                                                                          \
        struct TypeRegistration<ClassName>                                                                                                   \
        {                                                                                                                                    \
            inline static constexpr std::array<const ::msce::MemberInfo, BOOST_PP_VARIADIC_SIZE(__VA_ARGS__)> members = {                    \
                BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(MSCE_REFLECT_CLASS_MEMBER_WRP, ClassName, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)))}; \
            inline static constexpr msce::Type type = msce::Type(#ClassName,                                                                 \
                                                                 sizeof(ClassName),                                                          \
                                                                 ::msce::internal::compute_type_traits<ClassName>(),                         \
                                                                 std::span(members),                                                         \
                                                                 typeid(ClassName));                                                         \
            inline static constexpr const msce::Type &get_type()                                                                             \
            {                                                                                                                                \
                return type;                                                                                                                 \
            }                                                                                                                                \
            static void register_self()                                                                                                      \
            {                                                                                                                                \
                static const ::msce::Type &r = []() {                 Logger logger("StaticTypeRegistration");                                                                                        \
                ::msce::internal::get_g_reflection_types_registry().register_entry(#ClassName, ::std::cref(TypeRegistration<ClassName>::type)); \
                logger.log_info("Successfully reflected type '{}'", TypeRegistration<ClassName>::type.get_name_str());                          \
                    return TypeRegistration<ClassName>::type; }();                                                                                   \
            }                                                                                                                                \
            [[gnu::used]] TypeRegistration()                                                                                                 \
            {                                                                                                                                \
                register_self();                                                                                                             \
            }                                                                                                                                \
        };                                                                                                                                   \
        inline static TypeRegistration<ClassName> BOOST_PP_CAT(refl_t_reg, UID);                                                             \
    }

#define MSCE_REFLECT_CLASS(ClassName, ...) MSCE_REFLECT_CLASS_HELPER(ClassName, __COUNTER__, __VA_ARGS__)

#define MSCE_REFLECT_FUNDAMENTAL(TypeName) \
    MSCE_FUNDAMENTAL_HELPER(TypeName, __COUNTER__)

MSCE_REFLECT_FUNDAMENTAL(void)
MSCE_REFLECT_FUNDAMENTAL(void *)
MSCE_REFLECT_FUNDAMENTAL(bool)
MSCE_REFLECT_FUNDAMENTAL(char)
MSCE_REFLECT_FUNDAMENTAL(unsigned char)
MSCE_REFLECT_FUNDAMENTAL(char8_t)
MSCE_REFLECT_FUNDAMENTAL(char16_t)
MSCE_REFLECT_FUNDAMENTAL(char32_t)
MSCE_REFLECT_FUNDAMENTAL(short)
MSCE_REFLECT_FUNDAMENTAL(unsigned short)
MSCE_REFLECT_FUNDAMENTAL(int)
MSCE_REFLECT_FUNDAMENTAL(unsigned int)
MSCE_REFLECT_FUNDAMENTAL(long)
MSCE_REFLECT_FUNDAMENTAL(unsigned long)
MSCE_REFLECT_FUNDAMENTAL(long long)
MSCE_REFLECT_FUNDAMENTAL(unsigned long long)
MSCE_REFLECT_FUNDAMENTAL(float)
MSCE_REFLECT_FUNDAMENTAL(double)
MSCE_REFLECT_FUNDAMENTAL(long double)
MSCE_REFLECT_FUNDAMENTAL(const char *)
MSCE_REFLECT_FUNDAMENTAL(char *)
MSCE_REFLECT_FUNDAMENTAL(std::string)

#endif // MSCE_REFLECTION_H_