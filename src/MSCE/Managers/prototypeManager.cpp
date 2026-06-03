#include "prototypeManager.h"
#include <fstream>
#include <functional>

#if defined(RELEASE)
#include <cereal/archives/binary.hpp>
#else
#include <cereal/archives/json.hpp>
#endif

#include <cereal/types/memory.hpp>

using namespace std;

msce::PrototypeManager::PrototypeManager()
    : registered_factories_ref(get_g_registered_prototype_factories()),
      registered_prototypes_ref(get_g_registered_prototypes())
{
    logger.log_info("Initializing manager...");
}

msce::IPrototype *msce::PrototypeManager::deserialize_prototype(const string &path)
{
    ifstream file(path);
    if (!file)
    {
        logger.log_error("Couldn't open prototype file at '{}'", path);
        return nullptr;
    }

    try
    {
        std::unique_ptr<msce::IPrototype> proto;

#if defined(RELEASE)
        cereal::BinaryInputArchive ar(file);
        ar(proto);
#else
        cereal::JSONInputArchive ar(file);
        ar(proto);
#endif
        auto ptr = proto.get();
        this->prototypes_[proto->id] = std::move(proto);
        return ptr;
    }
    catch (const exception &e)
    {
        logger.log_error("Couldn't deserialize prototype at '{}': ", e.what());
        return nullptr;
    }
    catch (...)
    {
        file.close();
        return nullptr;
    }

    return nullptr;
}

void msce::PrototypeManager::serialize_prototype(const std::string &path, const std::string &id)
{
    if (!prototype_id_exists(id))
    {
        logger.log_error("Tried serializing non-manager-created prototype. Use create_new_prototype_instance() to create prototypes.");
        return;
    }

    ofstream file(path);
    if (!file)
    {
        logger.log_error("Couldn't create prototype cereal file at '{}'", path);
        return;
    }

    try
    {
        auto &uptr_ref = this->prototypes_[id];

#if defined(RELEASE)
        cereal::BinaryOutputArchive ar(file);
        ar(uptr_ref);
#else
        cereal::JSONOutputArchive ar(file);
        ar(uptr_ref);
#endif
    }
    catch (const exception &e)
    {
        logger.log_error("Couldn't serialize '{}' into file at '{}': {}", id, path, e.what());
    }
    catch (...)
    {
        file.close();
        return;
    }
}

msce::IPrototype *msce::PrototypeManager::get_prototype(const std::string &id)
{
    if (!this->prototypes_.contains(id))
    {
        logger.log_error("Prototype '{}' doesn't exist.", id);
        return nullptr;
    }

    return this->prototypes_[id].get();
}

unordered_set<msce::IPrototype *> msce::PrototypeManager::enumerate_prototypes()
{
    unordered_set<IPrototype *> result = unordered_set<IPrototype *>();
    for (const auto &[_, p] : this->prototypes_)
    {
        result.insert(p.get());
    }

    return result;
}

msce::IPrototype *msce::PrototypeManager::create_new_prototype_instance(const std::string &type, const std::string &id) noexcept
{
    if (!registered_factories_ref.is_registered(type))
    {
        logger.log_error("Error: Prototype with type '{}' wasn't ever registered!", type);
        return nullptr;
    }
    if (this->prototypes_.contains(id))
    {
        logger.log_error("Error: Prototype with id '{}' already exist!", id);
        return nullptr;
    }

    this->prototypes_[id] = registered_factories_ref.get_entry(type)();
    prototypes_[id]->id = id;

    return this->prototypes_[id].get();
}
bool msce::PrototypeManager::prototype_id_exists(const std::string &id) const noexcept
{
    return this->prototypes_.contains(id);
}
bool msce::PrototypeManager::prototype_type_registered(const std::string &type) const noexcept
{
    return registered_prototypes_ref.is_registered(type) && registered_prototypes_ref.is_registered(type);
}
bool msce::PrototypeManager::delete_prototype(const std::string &id) noexcept
{
    if (!prototype_id_exists(id))
        return false;

    this->prototypes_.erase(id);
    return true;
}
bool msce::PrototypeManager::delete_prototype(IPrototype *proto) noexcept
{
    return delete_prototype(proto->id);
};