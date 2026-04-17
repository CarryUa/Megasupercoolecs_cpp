#ifndef MSCE_I_PROTOTYPE_H_
#define MSCE_I_PROTOTYPE_H_
#include <string>
#include <MSCE/msce_macros.h>

namespace msce
{
    class IPrototype
    {
    public:
        std::string id = "Invalid default prototype!";

        virtual ~IPrototype() = default;

    public:
        friend class ::cereal::access;
        template <class Archive>
        void save(Archive &ar) const { ar(::cereal::make_nvp("id", id)); }
        template <class Archive>
        void load(Archive &ar) { ar(::cereal::make_nvp("id", id)); }
        static std::string_view get_unmangled_type_name() { return "IPrototype"; }
        static std::unordered_map<std::string_view, std::reference_wrapper<const std::type_info>> get_field_name_type_pairs_static() { return {std::pair(std::string_view("id"), std::reference_wrapper<const std::type_info>(get_member_type(&IPrototype::id)))}; }
        virtual std::unordered_map<std::string_view, std::reference_wrapper<const std::type_info>> get_field_name_type_pairs() const = 0;
        template <typename T>
        void set_field(const std::string &name, T value) noexcept
        {
            if (name == "id")
            {
                std::any a = value;
                if (a.type() != typeid(this->id))
                {
                    std::cerr << "Wrong type for " << typeid(this).name() << " object's '" << name << "' field. Expected '" << typeid(this->id).name() << "' but got '" << a.type().name() << "'!" << std::endl;
                    return;
                }
                this->id = std::any_cast<decltype(this->id)>(a);
            }
        }
        template <typename T>
        T get_field(const std::string &name) const
        {
            if (name == "id")
            {
                if (typeid(this->id) != typeid(T))
                {
                    std::cerr << "Wrong type for " << typeid(this).name() << " object's '" << name << "' field. Expected '" << typeid(this->id).name() << "' but got '" << typeid(T).name() << "'!" << std::endl;
                    throw std::runtime_error("Get field type mismatch.");
                }
                std::any a = this->id;
                return std::any_cast<T>(a);
            }
            throw std::runtime_error("Field '" + name + "' not found");
        }
    };

}
CEREAL_REGISTER_TYPE(msce::IPrototype)

#endif // MSCE_I_PROTOTYPE_H_