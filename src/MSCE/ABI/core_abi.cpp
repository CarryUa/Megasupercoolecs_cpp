#include "core_abi.h"
#include <MSCE/msce.h>

using namespace msce;

namespace
{
    static IPrototype *current_prototype_selection = nullptr;
    static const Type *current_prototype_type = nullptr;
    static std::vector<std::reference_wrapper<const MemberInfo>> current_prototype_fields;
    static std::unique_ptr<Enum> current_enum_selection = nullptr;
    static Logger proto_abi_logger("PrototypeManagerABI");
    static Logger enum_abi_logger("EnumManagerABI");
    static Logger logger("Engine User");

    PrototypeFieldTypes what_type_am_i(const std::type_info &type)
    {
        if (type == typeid(int))
            return PrototypeFieldTypes::INT;
        else if (EnumManager::instance->is_type_enum(type))
            return PrototypeFieldTypes::ENUM;
        else if (type == typeid(unsigned int))
            return PrototypeFieldTypes::UINT;
        else if (type == typeid(float))
            return PrototypeFieldTypes::FLOAT;
        else if (type == typeid(double))
            return PrototypeFieldTypes::DOUBLE;
        else if (type == typeid(std::string))
            return PrototypeFieldTypes::STRING;
        else if (type == typeid(char *))
            return PrototypeFieldTypes::STRING;

        return PrototypeFieldTypes::UNSUPPORTED;
    }
}

MSCE_ABI_EXPORT_ATTRIB void msce_initialize_engine()
{
    static SystemManager g_system_manager;
    g_system_manager.init_all_systems();
    static EntityManager g_entity_manager;
    // static ShaderManager g_shader_manager;
    static EventManager g_event_manager;
    static PrototypeManager g_prototype_manager;
    static ComponentManager g_component_manager;
    static EnumManager g_enum_manager;

    std::set_terminate(msce::internal::handle_terminate);
}

MSCE_ABI_EXPORT_ATTRIB void msce_update_engine()
{
    SystemManager::instance->update_all_systems();
}

MSCE_ABI_EXPORT_ATTRIB void msce_log_info(const char *message)
{
    logger.log_info(message);
}

MSCE_ABI_EXPORT_ATTRIB void msce_log_debug(const char *message)
{
    logger.log_debug(message);
}

MSCE_ABI_EXPORT_ATTRIB void msce_log_warning(const char *message)
{
    logger.log_warning(message);
}

MSCE_ABI_EXPORT_ATTRIB void msce_log_error(const char *message)
{
    logger.log_error(message);
}

MSCE_ABI_EXPORT_ATTRIB void msce_select_prototype(const char *id)
{
    if (!id)
        return;

    auto ptr = PrototypeManager::instance->get_prototype(id);
    if (!ptr)
    {
        proto_abi_logger.log_error("Prototype with id '{}' doesn't exist. Ignoring select...", id);
        return;
    }

    current_prototype_fields.clear();
    current_prototype_selection = ptr;
    current_prototype_type = &current_prototype_selection->get_type_info_polymorphic();

    for (auto &m : current_prototype_type->get_members())
    {
        current_prototype_fields.push_back(m);
    }
}

MSCE_ABI_EXPORT_ATTRIB void msce_get_selected_prototype_id(char *out_id, int out_size)
{
    if (!out_id || out_size <= 0)
        return;
    out_id[0] = '\0';

    if (!current_prototype_selection)
    {
        proto_abi_logger.log_error("Tried to get the id, but no prototype was selected!");
        return;
    }

    int buffer_size = std::min(current_prototype_selection->id.length(), static_cast<size_t>(out_size - 1));
    memcpy(out_id, current_prototype_selection->id.c_str(), buffer_size);
    out_id[buffer_size] = '\0';
}

MSCE_ABI_EXPORT_ATTRIB void msce_get_selected_prototype_fields_count(int *out_count)
{
    if (!current_prototype_selection)
    {
        proto_abi_logger.log_error("Tried to get the id, but no prototype was selected!");
        *out_count = -1;
        return;
    }

    *out_count = current_prototype_type->get_members().size();
}

MSCE_ABI_EXPORT_ATTRIB void msce_get_selected_prototype_field_name(int field_index, char *out_field_name, int out_size)
{
    if (!out_field_name || out_size <= 0)
        return;
    out_field_name[0] = '\0';

    if (!current_prototype_selection)
    {
        proto_abi_logger.log_error("Tried to get the id, but no prototype was selected!");
        return;
    }

    int fields_count = current_prototype_fields.size();

    if (field_index >= fields_count)
    {
        proto_abi_logger.log_error("Tried to get field name of field #{}, but there are only {}!", field_index + 1, fields_count);
        return;
    }

    const auto name = current_prototype_fields[field_index].get().get_name_str();

    int buffer_size = std::min(name.length(), static_cast<size_t>(out_size - 1));
    memcpy(out_field_name, name.data(), buffer_size);
    out_field_name[buffer_size] = '\0';
}

MSCE_ABI_EXPORT_ATTRIB void msce_get_selected_prototype_field_value(const char *field_name, char *out_field_value, int out_size, int *out_type_enum)
{
    if (!out_field_value || out_size <= 0)
        return;
    out_field_value[0] = '\0';

    if (!field_name)
        return;

    if (!current_prototype_selection)
    {
        proto_abi_logger.log_error("Tried to get the id, but no prototype was selected!");
        return;
    }
    try
    {
        auto type_enum = what_type_am_i(current_prototype_type->get_member_named(field_name).get_type().get_std_type());
        std::string str_v;

        switch (type_enum)
        {
        case PrototypeFieldTypes::UNSUPPORTED:
        {
            break;
        }
        case PrototypeFieldTypes::INT:
        {
            str_v = std::to_string(current_prototype_type->get_member_value<int>(reinterpret_cast<const void *>(current_prototype_selection), field_name));
            break;
        }
        case PrototypeFieldTypes::UINT:
        {
            str_v = std::to_string(current_prototype_type->get_member_value<unsigned int>(reinterpret_cast<const void *>(current_prototype_selection), field_name));
            break;
        }
        case PrototypeFieldTypes::ENUM:
        {
            str_v = std::to_string((current_prototype_type->get_member_value<int>(reinterpret_cast<const void *>(current_prototype_selection), field_name)));
            break;
        }
        case PrototypeFieldTypes::FLOAT:
        {
            str_v = std::to_string(current_prototype_type->get_member_value<float>(reinterpret_cast<const void *>(current_prototype_selection), field_name));
            break;
        }
        case PrototypeFieldTypes::DOUBLE:
        {
            str_v = std::to_string(current_prototype_type->get_member_value<double>(reinterpret_cast<const void *>(current_prototype_selection), field_name));
            break;
        }
        case PrototypeFieldTypes::STRING:
        {
            str_v = current_prototype_type->get_member_value<std::string>(reinterpret_cast<const void *>(current_prototype_selection), field_name);
            break;
        }
        }

        std::size_t buffer_size = std::min(str_v.length(), static_cast<size_t>(out_size - 1));
        memcpy(out_field_value, str_v.c_str(), buffer_size);
        out_field_value[buffer_size] = '\0';
        *out_type_enum = static_cast<int>(type_enum);
    }
    catch (...)
    {
        return;
    }
}

MSCE_ABI_EXPORT_ATTRIB void msce_set_selected_prototype_field_value(const char *field_name, const void *value, int type_enum)
{
    if (!field_name || !value)
        return;

    if (!current_prototype_selection)
    {
        proto_abi_logger.log_error("Tried to get the id, but no prototype was selected!");
        return;
    }
    PrototypeFieldTypes type = static_cast<PrototypeFieldTypes>(type_enum);
    std::any any_v;
    switch (type)
    {
    case PrototypeFieldTypes::UNSUPPORTED:
    {
        return;
    }
    case PrototypeFieldTypes::INVALID:
    {
        return;
    }
    case PrototypeFieldTypes::INT:
    {
        any_v = *static_cast<const int *>(value);
        current_prototype_type->set_member_value<int>(*current_prototype_selection, field_name, std::any_cast<int>(any_v));
        break;
    }
    case PrototypeFieldTypes::UINT:
    {
        any_v = *static_cast<const unsigned int *>(value);
        current_prototype_type->set_member_value<unsigned int>(*current_prototype_selection, field_name, std::any_cast<unsigned int>(any_v));
        break;
    }
    case PrototypeFieldTypes::ENUM:
    {
        int int_v = *static_cast<const int *>(value);

        auto en = EnumManager::instance->create_enum(current_prototype_type->get_member_named(field_name).get_type().get_name(), int_v);
        current_prototype_type->set_member_value<int>(reinterpret_cast<const void *>(current_prototype_selection), field_name, *current_prototype_type, int_v);

        break;
    }
    case PrototypeFieldTypes::FLOAT:
    {
        any_v = *static_cast<const float *>(value);
        current_prototype_type->set_member_value<float>(*current_prototype_selection, field_name, std::any_cast<float>(any_v));
        break;
    }
    case PrototypeFieldTypes::DOUBLE:
    {
        any_v = *static_cast<const double *>(value);
        current_prototype_type->set_member_value<double>(*current_prototype_selection, field_name, std::any_cast<double>(any_v));
        break;
    }
    case PrototypeFieldTypes::STRING:
    {
        any_v = std::string(static_cast<const char *>(value));
        current_prototype_type->set_member_value<std::string>(*current_prototype_selection, field_name, std::any_cast<std::string>(any_v));
        break;
    }
    }
}

MSCE_ABI_EXPORT_ATTRIB void msce_create_prototype_instance(const char *type, const char *new_id, bool *success)
{
    if (!success)
        return;

    *success = false;
    if (!type || !new_id)
        return;

    auto ptr = PrototypeManager::instance->create_new_prototype_instance(type, new_id);
    if (!ptr)
        return;

    *success = true;
}

MSCE_ABI_EXPORT_ATTRIB void msce_serialize_prototype(const char *dest_path, const char *id, bool *success)
{
    if (!success)
        return;
    *success = false;

    if (!dest_path || !id)
        return;

    auto ptr = PrototypeManager::instance->get_prototype(id);
    if (!ptr)
        return;

    PrototypeManager::instance->serialize_prototype(dest_path, id);

    *success = true;
}

MSCE_ABI_EXPORT_ATTRIB void msce_deserialize_prototype(const char *target_path, char *out_id, int out_id_size)
{
    if (out_id_size == 0 || !out_id)
        return;
    out_id[0] = '\0';

    if (!target_path)
        return;

    auto proto = PrototypeManager::instance->deserialize_prototype(target_path);
    if (!proto)
        return;

    int buffer_size = std::min(proto->id.length(), static_cast<size_t>(out_id_size - 1));
    memcpy(out_id, proto->id.c_str(), buffer_size);
    out_id[buffer_size] = '\0';
}

MSCE_ABI_EXPORT_ATTRIB void msce_select_enum(const char *enum_name)
{
    if (!enum_name)
        return;

    if (!EnumManager::instance->enum_exists(enum_name))
    {
        enum_abi_logger.log_warning("Enum with name '{}' does not exist. Ignoring select...", enum_name);
        return;
    }

    current_enum_selection.reset(new Enum(EnumManager::instance->create_enum(enum_name, 0)));
}

MSCE_ABI_EXPORT_ATTRIB void msce_get_selected_enum_value(int *out_value)
{
    if (!out_value)
        return;

    if (!current_enum_selection)
    {
        enum_abi_logger.log_error("No enum is selected.");
        return;
    }

    *out_value = current_enum_selection->get_value<int>();
}

MSCE_ABI_EXPORT_ATTRIB void msce_get_selected_enum_name(char *out_enum_name, int out_buffer_size)
{
    if (!out_enum_name || out_buffer_size <= 0)
        return;

    if (!current_enum_selection)
    {
        enum_abi_logger.log_error("No enum is selected.");
        return;
    }

    auto name = current_enum_selection->get_name();

    int bytes_needed = std::min(out_buffer_size - 1, static_cast<int>(name.length()));
    memcpy(out_enum_name, name.c_str(), bytes_needed);
    out_enum_name[bytes_needed] = '\0';
}

MSCE_ABI_EXPORT_ATTRIB void msce_get_selected_enum_enumerator_name(char *out_enumerator_name, int out_buffer_size)
{
    if (!out_enumerator_name || out_buffer_size <= 0)
        return;

    if (!current_enum_selection)
    {
        enum_abi_logger.log_error("No enum is selected.");
        return;
    }

    auto name = current_enum_selection->get_enumerator_name();

    int bytes_needed = std::min(out_buffer_size - 1, static_cast<int>(name.length()));
    memcpy(out_enumerator_name, name.c_str(), bytes_needed);
    out_enumerator_name[bytes_needed] = '\0';
}

MSCE_ABI_EXPORT_ATTRIB void msce_set_selected_enum_value(int value)
{
    if (!current_enum_selection)
    {
        enum_abi_logger.log_error("No enum is selected.");
        return;
    }

    current_enum_selection->set(value);
}

MSCE_ABI_EXPORT_ATTRIB void msce_set_selected_enum_enumerator_name(const char *enumerator_name)
{
    if (!enumerator_name)
        return;

    if (!current_enum_selection)
    {
        enum_abi_logger.log_error("No enum is selected.");
        return;
    }

    current_enum_selection->set(enumerator_name);
}
