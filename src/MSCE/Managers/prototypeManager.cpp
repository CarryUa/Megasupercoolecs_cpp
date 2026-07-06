#include "prototypeManager.h"
#include <fstream>
#include <functional>

#include <MSCE/Serialization/serializer.h>
#include <MSCE/Events/prototypeEvents.h>
#include <MSCE/Managers/eventManager.h>

using namespace std;

namespace
{
    constexpr const char *prototype_extension = ".msceproto";
}

void msce::PrototypeManager::load_all_prototypes()
{
    logger.log_info("Loading prototypes...");
    uint successes = 0, failures = 0;
    for (auto e : std::filesystem::recursive_directory_iterator(Platform::get_absolute_executable_directory_path()))
    {
        if (!e.is_regular_file())
            continue;

        if (e.path().extension() == prototype_extension)
        {
            IPrototype *p = deserialize_prototype(e.path());

            if (p == nullptr)
                failures++;
            else
                successes++;
        }
    }
    logger.log_info("Finished loading prototypes. total: {} | successes: {} | failures: {}", successes + failures, successes, failures);
}

msce::PrototypeManager::PrototypeManager()
    : registered_factories_ref(get_g_registered_prototype_factories()),
      registered_prototypes_ref(get_g_registered_prototypes())
{
    logger.log_info("Initializing manager...");

    PrototypeLoadingStartingEvent start_ev;
    EventManager::instance->fire(start_ev);
    load_all_prototypes();
    PrototypeLoadingFinishedEvent fin_ev;
    EventManager::instance->fire(fin_ev);
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

        Serializer::deserialize(proto, file);

        auto ptr = proto.get();
        this->prototypes_[proto->id] = std::move(proto);
        return ptr;
    }
    catch (const exception &e)
    {
        logger.log_error("Couldn't deserialize prototype at '{}': {}", path, e.what());
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

        Serializer::serialize(uptr_ref, file);
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