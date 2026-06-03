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

    template <typename T>
    struct TypeRegistration;

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
        COPY_ASIGNABLE = 1 << 12,
        MOVE_ASIGNABLE = 1 << 13,
    };

    namespace internal
    {
        template <typename T>
        inline uint16_t compute_type_traits()
        {
            static Logger l("Trait checker");
            l.log_debug("Generating traits for {}", typeid(T).name());

            uint16_t flags = 0;
            if (std::is_arithmetic_v<T>)
            {
                l.log_debug("{} is numerical", typeid(T).name());
                if (std::is_integral_v<T>)
                {
                    flags |= static_cast<uint16_t>(TypeTraitFlags::INTEGRAL);
                    l.log_debug("{} is integral", typeid(T).name());
                }
                else
                {
                    flags |= static_cast<uint16_t>(TypeTraitFlags::FLOATING_POINT);
                    l.log_debug("{} is fp", typeid(T).name());
                }

                if (!std::is_signed_v<T>)
                {
                    flags |= static_cast<uint16_t>(TypeTraitFlags::UNSIGNED);
                    l.log_debug("{} is unsigned", typeid(T).name());
                }
            }
            else if (std::is_class_v<T>)
            {
                flags |= static_cast<uint16_t>(TypeTraitFlags::CLASS);
            }
            else if (std::is_invocable_v<T>)
            {
                flags |= static_cast<uint16_t>(TypeTraitFlags::FUNCTION);
            }
            // Important to notice here, that I explicitly exclude function and member function pointers from POINTER category.
            // They fall into FUNCTION category, since I use std::is_invocable_v to check for functions.
            else if (std::is_pointer_v<T>)
            {
                flags |= static_cast<uint16_t>(TypeTraitFlags::POINTER);
                if (std::is_member_object_pointer_v<T>)
                    flags |= static_cast<uint16_t>(TypeTraitFlags::MEMBER_OBJECT_POINTER);
            }
            else if (std::is_reference_v<T>)
            {
                flags |= static_cast<uint16_t>(TypeTraitFlags::REFERENCE);
                if (std::is_lvalue_reference_v<T>)
                    flags |= static_cast<uint16_t>(TypeTraitFlags::LVALUE);
                else
                    flags |= static_cast<uint16_t>(TypeTraitFlags::RVALUE);
            }
            else if (std::is_array_v<T>)
            {
                flags |= static_cast<uint16_t>(TypeTraitFlags::ARRAY);
                if (std::is_bounded_array_v<T>)
                    flags |= static_cast<uint16_t>(TypeTraitFlags::BOUNDED);
            }
            else if (std::is_enum_v<T>)
            {
                flags |= static_cast<uint16_t>(TypeTraitFlags::ENUM);
            }

            else
                flags |= static_cast<uint16_t>(TypeTraitFlags::VOID);

            /* SHARED stuff */
            if (std::is_const_v<T>)
                flags |= static_cast<uint16_t>(TypeTraitFlags::CONST);
            if (std::is_volatile_v<T>)
                flags |= static_cast<uint16_t>(TypeTraitFlags::VOLATILE);
            if (std::is_default_constructible_v<T>)
                flags |= static_cast<uint16_t>(TypeTraitFlags::DEFAULT_CONSTRUCTABLE);
            if (std::is_copy_constructible_v<T>)
                flags |= static_cast<uint16_t>(TypeTraitFlags::COPY_CONSTRUCTABLE);
            if (std::is_move_constructible_v<T>)
                flags |= static_cast<uint16_t>(TypeTraitFlags::MOVE_CONSTRUCTABLE);
            if (std::is_move_assignable_v<T>)
                flags |= static_cast<uint16_t>(TypeTraitFlags::MOVE_ASIGNABLE);
            if (std::is_copy_assignable_v<T>)
                flags |= static_cast<uint16_t>(TypeTraitFlags::COPY_ASIGNABLE);

            l.log_debug("{} flags are {:b}", typeid(T).name(), flags);

            return flags;
        }

        inline Registry<std::string, std::reference_wrapper<const Type>> &get_g_reflection_types_registry()
        {
            static Registry<std::string, std::reference_wrapper<const Type>> reg;
            return reg;
        }
    }

    class MemberInfo
    {
    private:
        inline static Logger logger = Logger("Reflection(MemberInfo)");

        /// @brief Reference to @ref msce::Type of this type.
        const Type &type_info_;
        /// @brief Human-readable name of this member.
        const char *name_;
        /// @brief The offset from start of the container object, where this member is stored.
        const uint32_t offset_;

    public:
        MemberInfo(const char *name, uint32_t offset, const Type &type);

        template <typename TValue, ClassWithReflection TObject>
        TValue get_value(const TObject &object) const;
        template <typename TValue, ClassWithReflection TObject>
        void set_value(const TObject &objecr, TValue value) const;

        template <typename TValue, ClassWithReflection TObject>
        TValue *get_ptr(const TObject &object) const;
        template <ClassWithReflection TObject>
        void *get_ptr(const TObject &object) const;
        bool operator==(const MemberInfo &other) const;

        uint32_t get_offset() const;

        constexpr const char *get_name() const;
        std::string get_name_str() const;
    };

    class Type
    {
    private:
        inline static const Logger logger = Logger("Reflection(Type)");

        /// @brief std::type_info reference of underlying type.
        const std::type_info &std_type_;
        /// @brief Human-readable name of this type.
        const char *name_;
        /// @brief The size of this type.
        const uint32_t size_;
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
        const std::span<MemberInfo> members_;

    public:
        constexpr Type(const char *name, uint32_t size, uint16_t trait_flags, std::span<MemberInfo> members, const std::type_info &std_type) : name_(name),
                                                                                                                                               size_(size),
                                                                                                                                               traits_(trait_flags),
                                                                                                                                               members_(members),
                                                                                                                                               std_type_(std_type)

        {
        }

        const std::type_info &get_std_type() const;
        const char *get_name() const;
        std::string get_name_str() const;

        bool has_member(const MemberInfo &member) const;
        bool has_member_named(const char *name) const;
        const MemberInfo &get_member_named(const char *name) const;
        template <typename TValue, ClassWithReflection TObject>
        TValue get_member_value(const TObject &object, const char *member_name) const;

        template <typename TValue, ClassWithReflection TObject>
        void set_member_value(const TObject &object, const char *member_name, TValue value) const;

        uint32_t get_size() const;

        bool is_numeric() const;
        bool is_integer() const;
        bool is_floating_point() const;
        bool is_unsigned() const;

        bool is_class() const;

        bool is_pointer() const;
        bool is_member_object_pointer() const;

        bool is_reference() const;
        bool is_lvalue_reference() const;
        bool is_rvalue_reference() const;

        bool is_array() const;
        bool is_bounded_array() const;

        bool is_enum() const;
        bool is_function() const;
        bool is_void() const;

        bool is_const() const;
        bool is_volatile() const;

        bool is_default_constructible() const;
        bool is_copy_constructible() const;
        bool is_move_constructible() const;

        bool is_copy_asignable() const;
        bool is_move_asignable() const;

        bool operator==(const Type &other) const;
    };
    template <typename TValue, ClassWithReflection TObject>
    inline TValue MemberInfo::get_value(const TObject &object) const
    {
        if (!TObject::type_info.has_member(*this))
            throw std::runtime_error("Tried to get value of member '" + this->get_name_str() + "' from '" + TObject::type_info.get_name_str() + "', but said object doesn't have said member.");

        auto m_addr = reinterpret_cast<uintptr_t>(&object) + this->offset_;
        return *reinterpret_cast<TValue *>(m_addr);
    }
    template <typename TValue, ClassWithReflection TObject>
    inline void MemberInfo::set_value(const TObject &object, TValue value) const
    {
        if (!TObject::type_info.has_member(*this))
            throw std::runtime_error("Tried to set value of member '" + this->get_name_str() + "' in '" + TObject::type_info.get_name_str() + "', but said object doesn't have said member.");

        auto m = this->get_ptr<TValue>(object);
        if (!m)
        {
            logger.log_error("Tried to set value of member '{}' in object {}, but said object has no such member", this->get_name_str(), TObject::type_info.get_name_str());
            return;
        }
        *m = value;
    }
    template <typename TValue, ClassWithReflection TObject>
    inline TValue *MemberInfo::get_ptr(const TObject &object) const
    {
        if (this->type_info_.get_std_type() != typeid(TValue))
        {
            logger.log_error("Tried to set value of member '{}' in object {}, but said object has no such member", this->get_name_str(), TObject::type_info.get_name_str());
            return nullptr;
        }

        return reinterpret_cast<TValue *>(this->get_ptr(object));
    }
    template <ClassWithReflection TObject>
    inline void *MemberInfo::get_ptr(const TObject &object) const
    {

        if (!TObject::type_info.has_member(*this))
        {
            logger.log_error("Tried to set value of member '{}' in object {}, but said object has no such member", this->get_name_str(), TObject::type_info.get_name_str());
            return nullptr;
        }
        if (this->offset_ > TObject::type_info.get_size())
            return nullptr;

        auto m_addr = reinterpret_cast<uintptr_t>(&object) + this->offset_;
        return reinterpret_cast<void *>(m_addr);
    }
    template <typename TValue, ClassWithReflection TObject>
    inline TValue Type::get_member_value(const TObject &object, const char *member_name) const
    {
        const auto &m = this->get_member_named(member_name);
        return m.get_value<TValue>(object);
    }

    template <typename TValue, ClassWithReflection TObject>
    inline void Type::set_member_value(const TObject &object, const char *member_name, TValue value) const
    {
        const auto &m = this->get_member_named(member_name);
        m.set_value(object, value);
    }

    template <typename T>
    const Type &get_reflection_of_type();
    const Type &get_reflection_of_type(const std::string &name);
    const Type &get_reflection_of_type(const std::type_info &std_type);

}

#define MSCE_DEFINE_REFLECTED_OBJECT() \
    static const ::msce::Type &type_info;

#define MSCE_DEFINE_AND_INSTANTIATE_REFLECTED_NON_CLASS_HELPER(ReflectedType, RTUID)                                                                                                  \
    namespace                                                                                                                                                                         \
    {                                                                                                                                                                                 \
        ::msce::Type BOOST_PP_CAT(registered_type_, RTUID)(#ReflectedType, sizeof(ReflectedType), ::msce::internal::compute_type_traits<ReflectedType>(), {}, typeid(ReflectedType)); \
    }                                                                                                                                                                                 \
    template <>                                                                                                                                                                       \
    const ::msce::Type & ::msce::get_reflection_of_type<ReflectedType>()                                                                                                              \
    {                                                                                                                                                                                 \
        return BOOST_PP_CAT(registered_type_, RTUID);                                                                                                                                 \
    }                                                                                                                                                                                 \
    namespace msce                                                                                                                                                                    \
    {                                                                                                                                                                                 \
        template <>                                                                                                                                                                   \
        class TypeRegistration<ReflectedType>                                                                                                                                         \
        {                                                                                                                                                                             \
        public:                                                                                                                                                                       \
            [[gnu::used]] TypeRegistration()                                                                                                                                          \
            {                                                                                                                                                                         \
                static Logger logger("StaticReflectionTypeRegistration");                                                                                                             \
                ::msce::internal::get_g_reflection_types_registry().register_entry(#ReflectedType, std::cref(BOOST_PP_CAT(registered_type_, RTUID)));                                 \
                logger.log_info("Registered type '{}'", #ReflectedType);                                                                                                              \
            }                                                                                                                                                                         \
        };                                                                                                                                                                            \
        TypeRegistration<ReflectedType> BOOST_PP_CAT(reg_, RTUID);                                                                                                                    \
    }
#define MSCE_DEFINE_AND_INSTANTIATE_REFLECTED_NON_CLASS(ReflectedType) \
    MSCE_DEFINE_AND_INSTANTIATE_REFLECTED_NON_CLASS_HELPER(ReflectedType, __COUNTER__)

#define MSCE_INITIALIZE_REFLECTED_OBJECT_MEMBER_HELPER(ReflectedType, MemberName) \
    ::msce::MemberInfo(#MemberName, offsetof(ReflectedType, MemberName), ::msce::get_reflection_of_type(typeid(decltype(ReflectedType::MemberName))))

#define MSCE_MSCE_INITIALIZE_REFLECTED_OBJECT_MEMBER_WRAPPER(r, data, x) \
    MSCE_INITIALIZE_REFLECTED_OBJECT_MEMBER_HELPER(data, x)

#define MSCE_INITIALIZE_REFLECTED_OBJECT_HELPER(ReflectedType, RTUID, ...)                                                                                                                                                                                                                             \
    namespace                                                                                                                                                                                                                                                                                          \
    {                                                                                                                                                                                                                                                                                                  \
        ::std::array<::msce::MemberInfo, BOOST_PP_VARIADIC_SIZE(__VA_ARGS__)> BOOST_PP_CAT(BOOST_PP_CAT(registered_type_, RTUID), _members) = {BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TRANSFORM(MSCE_MSCE_INITIALIZE_REFLECTED_OBJECT_MEMBER_WRAPPER, ReflectedType, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)))}; \
        ::msce::Type BOOST_PP_CAT(registered_type_, RTUID)(#ReflectedType, sizeof(ReflectedType), ::msce::internal::compute_type_traits<ReflectedType>(), BOOST_PP_CAT(BOOST_PP_CAT(registered_type_, RTUID), _members), typeid(ReflectedType));                                                       \
    }                                                                                                                                                                                                                                                                                                  \
    namespace msce                                                                                                                                                                                                                                                                                     \
    {                                                                                                                                                                                                                                                                                                  \
        template <>                                                                                                                                                                                                                                                                                    \
        class TypeRegistration<ReflectedType>                                                                                                                                                                                                                                                          \
        {                                                                                                                                                                                                                                                                                              \
        public:                                                                                                                                                                                                                                                                                        \
            [[gnu::used]] TypeRegistration()                                                                                                                                                                                                                                                           \
            {                                                                                                                                                                                                                                                                                          \
                static Logger logger("StaticReflectionTypeRegistration");                                                                                                                                                                                                                              \
                ::msce::internal::get_g_reflection_types_registry().register_entry(#ReflectedType, std::cref(BOOST_PP_CAT(registered_type_, RTUID)));                                                                                                                                                  \
                logger.log_info("Registered type '{}'", #ReflectedType);                                                                                                                                                                                                                               \
            }                                                                                                                                                                                                                                                                                          \
        };                                                                                                                                                                                                                                                                                             \
        TypeRegistration<ReflectedType> BOOST_PP_CAT(reg_, RTUID);                                                                                                                                                                                                                                     \
    }                                                                                                                                                                                                                                                                                                  \
    const ::msce::Type &ReflectedType::type_info = BOOST_PP_CAT(registered_type_, RTUID);                                                                                                                                                                                                              \
    template <>                                                                                                                                                                                                                                                                                        \
    const ::msce::Type & ::msce::get_reflection_of_type<ReflectedType>()                                                                                                                                                                                                                               \
    {                                                                                                                                                                                                                                                                                                  \
        return ReflectedType::type_info;                                                                                                                                                                                                                                                               \
    }

/**
 * @brief Initializes the type_info reference IN A CPP SOURCE FILE.
 * @note If you want to reflect parent objects, use MSCE_INITIALIZE_DERIVED_REFLECTED_OBJECT
 * @warning May be used ONLY in cpp source file.
 */
#define MSCE_INITIALIZE_REFLECTED_OBJECT(ReflectedType, ...) \
    MSCE_INITIALIZE_REFLECTED_OBJECT_HELPER(ReflectedType, __COUNTER__, __VA_ARGS__)

/**
 * @brief Initializes the type_info reference IN A CPP SOURCE FILE. Also adds object's parents to ther
 * @warning May be used ONLY in cpp source file.
 */
#define MSCE_INITIALIZE_DERIVED_REFLECTED_OBJECT(ReflectedType, ParentReflectedType, ...)
#endif // MSCE_REFLECTION_H_