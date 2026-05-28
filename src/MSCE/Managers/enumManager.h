#ifndef MSCE_ENUM_MANAGER_H_
#define MSCE_ENUM_MANAGER_H_
#include <MSCE/Types/Collections/registry.hpp>
#include <MSCE/Types/singleton.hpp>
#include <MSCE/Types/enum.hpp>

namespace msce
{
    /**
     * @brief Manages the @ref msce::Enum
     * @note @ref msce::Enum is mostly used by outside programs trough ABI, you can and should use normal enums for most use cases.
     */
    class EnumManager : public Singleton<EnumManager>
    {
    private:
        /// @brief Logger instance for EnumManager.
        inline static Logger logger = Logger("EnumManager");
        /// @brief Reference to global enum factories registry. @ref msce::get_g_enum_factories_registry().
        Registry<std::string, std::function<Enum()>> &registered_enum_factories_ref_;

    public:
        /// @brief Default constructor.
        EnumManager();

        /**
         * @brief Creates a generic enum type.
         * @param enum_name The actual enum(as in keyword) name.
         * @param value Value enumerator name.
         */
        Enum create_enum(const std::string &enum_name, const std::string &value);
        /**
         * @brief Creates a generic enum type.
         * @param enum_name The actual enum(as in keyword) name.
         * @param value Value enumerator name.
         */
        Enum create_enum(const std::string &enum_name, int64_t value);
    };
}

#endif // MSCE_ENUM_MANAGER_H_