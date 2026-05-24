#ifndef MSCE_PROTOTYPE_MANAGER_H_
#define MSCE_PROTOTYPE_MANAGER_H_
#include <memory>
#include <unordered_map>
#include <functional>

#include <MSCE/Types/singleton.hpp>
#include <MSCE/Types/Collections/registry.hpp>
#include <MSCE/Prototypes/prototype.hpp>
#include <MSCE/logger.h>

namespace msce
{
    class PrototypeManager : public Singleton<PrototypeManager>
    {
    private:
        std::unordered_map<std::string, std::unique_ptr<IPrototype>> prototypes_;
        inline static Logger logger = Logger("PrototypeManager");

    public:
        PrototypeManager();

        Registry<std::string, std::function<std::unique_ptr<IPrototype>()>> &registered_factories_ref;
        Registry<std::string, std::reference_wrapper<const std::type_info>> &registered_prototypes_ref;
        /// @brief Tries do deserialize cereal file and create a prototype based on it.
        /// @param path The path to cereal file.
        void deserialize_prototype(const std::string &path);

        /// @brief Tries do serialize a prototype.
        /// @param path The path to cereal file to be created.
        /// @param prototype The prototype uptr that needs to be serialized.
        void serialize_prototype(const std::string &path, const std::unique_ptr<IPrototype> &prototype);

        /// @brief Returns the prototype with provided id.
        /// @param id The id of the prototype
        /// @return A pointer to the prototype or nullptr
        IPrototype *get_prototype(const std::string &id);

        /// @brief Returns the prototype with provided id, if it can be cast to the provided type.
        /// @tparam TProto Prototype type to be returned.
        /// @param id The id of the prototype
        /// @return A pointer to the prototype or nullptr
        template <typename TProto>
        TProto *get_prototype(const std::string &id);

        /// @return Returns an unordered set of all prototypes pointers.
        std::unordered_set<IPrototype *> enumerate_prototypes();

        /// @return Returns an unordered set of pointers to all TProto* castable prototypes.
        template <typename TProto>
        std::unordered_set<TProto *> enumerate_prototypes();

        /// @brief Creates registered prototype by its typename string at runtime.
        /// @param type The type of the prototype, that it was registered with.
        /// @param id Id that will be asigned to the prototype.
        /// @return Pointer to newly-created prototype or nullptr.
        IPrototype *instantiate_prototype(const std::string &type, const std::string &id) noexcept;

        /// @brief Creates registered prototype by its type-name string at runtime.
        /// @tparam TProto Type that prototype will be cast to.
        /// @param type The type of the prototype, that it was registered with.
        /// @param id Id that will be asigned to the prototype.
        /// @return Pointer to newly-created prototype or nullptr.
        template <typename TProto>
        TProto *instantiate_prototype(const std::string &type, const std::string &id) noexcept;

        /// @param id Try and guess.
        /// @return True if prototype with given ID exists, and false otherwise.
        bool prototype_id_exists(const std::string &id) const noexcept;

        /// @param type Try and guess.
        /// @return True if prototype with given type was registered in bouth regestries, and false otherwise.
        bool prototype_type_registered(const std::string &type) const noexcept;

        /// @param id Try and guess.
        /// @return True if prototype was successfully deleted, and false otherwise.
        bool delete_prototype(const std::string &id) noexcept;

        /// @brief This variant also sets proto argument to nullptr if deleted successfully.
        /// @param proto Pointer to the prototype to be deleted.
        /// @return True if prototype was successfully deleted, and false otherwise.
        bool delete_prototype(IPrototype *proto) noexcept;
    };

    template <typename TProto>
    TProto *PrototypeManager::get_prototype(const std::string &id)
    {
        if (!this->prototypes_.contains(id))
        {
            logger.log_warning("[PROTO]: Prototype with id '{}' doesn't exist", id);
            return nullptr;
        }
        TProto *p = dynamic_cast<TProto *>(this->prototypes_[id].get());
        if (p == nullptr)
        {
            logger.log_warning("[PROTO]: Prototype with id '{}' doesn't match type provided: {}", id, typeid(TProto).name());
            return nullptr;
        }

        return p;
    }

    template <typename TProto>
    std::unordered_set<TProto *> PrototypeManager::enumerate_prototypes()
    {
        std::unordered_set<TProto *> result = std::unordered_set<TProto *>();
        for (const auto &[_, p] : this->prototypes_)
        {
            TProto *cast_p = dynamic_cast<TProto *>(p.get());
            if (cast_p == nullptr)
                continue;

            result.insert(cast_p);
        }

        return result;
    };

    template <typename TProto>
    TProto *PrototypeManager::instantiate_prototype(const std::string &type, const std::string &id) noexcept
    {
        if (!registered_factories_ref.is_registered(type))
        {
            logger.log_error("Prototype with type '{} ' wasn't ever registered!", type);
            return nullptr;
        }
        if (this->prototypes_.contains(id))
        {
            logger.log_error("Prototype with id '{}' already exist!", id);
            return nullptr;
        }

        this->prototypes_[id] = registered_factories_ref.get_entry(type)();
        this->prototypes_[id]->id = id;

        auto ptr = dynamic_cast<TProto *>(this->prototypes_[id].get());
        if (ptr == nullptr)
        {
            logger.log_error("Prototype with type '{}' cant exist!", type);
            return nullptr;
        }

        return ptr;
    }
}
#endif // MSCE_PROTOTYPE_MANAGER_H_