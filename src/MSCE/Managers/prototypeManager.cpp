#include "prototypeManager.h"
#include <fstream>
#include <functional>
#ifdef DEBUG
#include <cereal/archives/json.hpp>
#endif
#ifdef RELEASE
#include <cereal/archives/binary.hpp>
#endif
#include <cereal/types/memory.hpp>

using namespace std;

msce::PrototypeManager::PrototypeManager()
    : registered_factories_ref(get_g_registered_prototype_factories()),
      registered_prototypes_ref(get_g_registered_prototypes())
{
}

void msce::PrototypeManager::deserialize_prototype(const string &path)
{
    ifstream file(path);
    if (!file)
    {
        logger.log_error("Couldn't open prototype file at '{}'", path);
        return;
    }

    try
    {
        std::unique_ptr<msce::IPrototype> proto;

#ifdef DEBUG
        cereal::JSONInputArchive ar(file);
        ar(proto);
#endif
#ifdef RELEASE
        cereal::BinaryInputArchive ar(file);
        ar(proto);
#endif

        this->prototypes_[proto->id] = std::move(proto);
    }
    catch (const exception &e)
    {
        logger.log_error("Couldn't deserialize prototype at '{}': ", e.what());
    }
    catch (...)
    {
        file.close();
        return;
    }
}

void msce::PrototypeManager::serialize_prototype(const std::string &path, const unique_ptr<IPrototype> &prototype)
{
    ofstream file(path);
    if (!file)
    {
        logger.log_error("Couldn't create prototype cereal file at '{}'", path);
        return;
    }

    try
    {
#ifdef DEBUG
        cereal::JSONOutputArchive ar(file);
        ar(prototype);
#endif
#ifdef RELEASE
        cereal::BinaryOutputArchive ar(file);
        ar(prototype);
#endif
    }
    catch (const exception &e)
    {
        logger.log_error("Couldn't serialize '{}' into file at '{}': {}", prototype->id, path, e.what());
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

msce::IPrototype *msce::PrototypeManager::instantiate_prototype(const std::string &type, const std::string &id) noexcept
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