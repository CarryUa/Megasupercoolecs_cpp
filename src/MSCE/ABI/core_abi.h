#ifndef MSCE_CORE_ABI_H_
#define MSCE_CORE_ABI_H_

#include "abi_macros.h"

// inline static constexpr int max_prototype_field_types_value = 6;
enum class PrototypeFieldTypes : int
{
    INVALID = -1,
    UNSUPPORTED = 0,
    INT = 1,
    UINT = 2,
    FLOAT = 3,
    DOUBLE = 4,
    STRING = 5,
    ENUM = 6,
};

extern "C"
{
#pragma region engine
    MSCE_ABI_EXPORT_ATTRIB void msce_initialize_engine();
    MSCE_ABI_EXPORT_ATTRIB void msce_update_engine();
#pragma endregion

#pragma region logger
    MSCE_ABI_EXPORT_ATTRIB void msce_log_info(const char *message);
    MSCE_ABI_EXPORT_ATTRIB void msce_log_debug(const char *message);
    MSCE_ABI_EXPORT_ATTRIB void msce_log_warning(const char *message);
    MSCE_ABI_EXPORT_ATTRIB void msce_log_error(const char *message);
#pragma endregion

#pragma region prototypes
    MSCE_ABI_EXPORT_ATTRIB void msce_select_prototype(const char *id);
    MSCE_ABI_EXPORT_ATTRIB void msce_get_selected_prototype_id(char *out_id, int out_size);
    MSCE_ABI_EXPORT_ATTRIB void msce_get_selected_prototype_fields_count(int *out_count);
    MSCE_ABI_EXPORT_ATTRIB void msce_get_selected_prototype_field_name(int field_index, char *out_field_name, int out_size);
    MSCE_ABI_EXPORT_ATTRIB void msce_get_selected_prototype_field_value(const char *field_name, char *out_field_value, int out_size, int *out_type_enum);
    MSCE_ABI_EXPORT_ATTRIB void msce_set_selected_prototype_field_value(const char *field_name, const void *value, int type_enum);

    MSCE_ABI_EXPORT_ATTRIB void msce_create_prototype_instance(const char *type, const char *new_id, bool *success);
    MSCE_ABI_EXPORT_ATTRIB void msce_serialize_prototype(const char *dest_path, const char *id, bool *success);
    MSCE_ABI_EXPORT_ATTRIB void msce_deserialize_prototype(const char *target_path, char *out_id, int out_id_size);
#pragma endregion

#pragma region enums

    MSCE_ABI_EXPORT_ATTRIB void msce_select_enum(const char *enum_name);
    MSCE_ABI_EXPORT_ATTRIB void msce_get_selected_enum_value(int *out_value);
    MSCE_ABI_EXPORT_ATTRIB void msce_get_selected_enum_name(char *out_enum_name, int out_buffer_size);
    MSCE_ABI_EXPORT_ATTRIB void msce_get_selected_enum_enumerator_name(char *out_enumerator_name, int out_buffer_size);
    MSCE_ABI_EXPORT_ATTRIB void msce_set_selected_enum_value(int value);
    MSCE_ABI_EXPORT_ATTRIB void msce_set_selected_enum_enumerator_name(const char *enumerator_name);

#pragma endregion
}

#endif // MSCE_CORE_ABI_H_