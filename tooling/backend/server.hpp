#pragma once

#include <httplib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <cmath>
#include <cctype>
#include <stdexcept>
#include <cstdint>

#include <cereal/archives/json.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/vector.hpp>
#include <nlohmann/json.hpp>

#include <MSCE/msce.h>

using json = nlohmann::json;

inline msce::Logger logger("Server");
inline std::string base_type_name(const msce::Type &type);

struct LogMessagePayload
{
  std::string message;
  MSCE_CEREAL_GENERATE_SERIALIZE_METHODS(message);
};

struct GetTypeFieldsBody
{
  std::string type;

  MSCE_CEREAL_GENERATE_SERIALIZE_METHODS(type);
};

struct FieldInfo
{
  std::string name;
  std::string type;
  bool is_class = false;
  bool is_enum = false;

  template <class Archive> void serialize(Archive &ar)
  {
    ar(CEREAL_NVP(name), CEREAL_NVP(type), CEREAL_NVP(is_class),
       CEREAL_NVP(is_enum));
  }
};

// ---------- helpers ----------

bool types_compatible(const msce::Type &type, json value,
                      std::vector<std::string> &errs)
{
  const std::string tname = type.get_name_str();

  if (type.get_std_type() == typeid(bool))
  {
    if (!value.is_boolean())
    {
      errs.push_back(
          std::format("Types {} and {} are incompatible. {} is not a boolean",
                      tname, value.type_name(), value.type_name()));
      return false;
    }
    return true;
  }

  if (type.get_std_type() == typeid(std::string))
  {
    if (!value.is_string())
    {
      errs.push_back(
          std::format("Types {} and {} are incompatible. {} is not a string",
                      tname, value.type_name(), value.type_name()));
      return false;
    }
    return true;
  }

  if (type.is_numeric())
  {
    if (!value.is_number())
    {
      errs.push_back(
          std::format("Types {} and {} are incompatible. {} is not numeric",
                      tname, value.type_name(), value.type_name()));
      return false;
    }
    return true;
  }

  if (type.is_enum())
  {
    if (value.is_number_integer() || value.is_string()) return true;

    errs.push_back(std::format(
        "Types {} and {} are incompatible. {} is not a valid enum value", tname,
        value.type_name(), value.type_name()));
    return false;
  }

  if (tname.rfind("std::vector<", 0) == 0)
  {
    if (!value.is_array())
    {
      errs.push_back(
          std::format("Types {} and {} are incompatible. {} is not an array",
                      tname, value.type_name(), value.type_name()));
      return false;
    }

    std::string elem_name = tname.substr(tname.find('<') + 1);
    if (!elem_name.empty() && elem_name.back() == '>') elem_name.pop_back();
    while (!elem_name.empty() && std::isspace((unsigned char)elem_name.front()))
      elem_name.erase(elem_name.begin());
    while (!elem_name.empty() && std::isspace((unsigned char)elem_name.back()))
      elem_name.pop_back();

    msce::Type const *elem_type = nullptr;
    try
    {
      elem_type = &msce::get_reflection_of_type(elem_name);
    }
    catch (...)
    {
      errs.push_back(std::format("Types {} and {} are incompatible. Element "
                                 "type '{}' is not reflected",
                                 tname, value.type_name(), elem_name));
      return false;
    }

    for (const auto &el : value)
    {
      if (!types_compatible(*elem_type, el, errs)) return false;
    }
    return true;
  }

  if (type == typeof(msce::internal::TypeComponentMap))
  {
    if (!value.is_object())
    {
      errs.push_back(
          std::format("Types {} and {} are incompatible. {} is not an object",
                      tname, value.type_name(), value.type_name()));
      return false;
    }
    for (auto &[key, val] : value.items())
    {
      msce::Type const *ctype = nullptr;
      try
      {
        ctype = &msce::get_reflection_of_type(key);
      }
      catch (...)
      {
        errs.push_back(std::format("Unknown component type '{}'", key));
        return false;
      }
      if (!types_compatible(*ctype, val, errs)) return false;
    }
    return true;
  }

  if (type.is_class())
  {
    if (!value.is_object())
    {
      errs.push_back(
          std::format("Types {} and {} are incompatible. {} is not an object",
                      tname, value.type_name(), value.type_name()));
      return false;
    }

    for (auto &[key, val] : value.items())
    {
      if (!type.has_member_named(key.c_str()))
      {
        errs.push_back(
            std::format("Field '{}' does not exist in type {}", key, tname));
        return false;
      }

      const msce::Type &mtype = type.get_member_named(key.c_str()).get_type();
      if (!types_compatible(mtype, val, errs)) return false;
    }
    return true;
  }

  if (type.is_array())
  {
    if (!value.is_array())
    {
      errs.push_back(
          std::format("Types {} and {} are incompatible. {} is not an array",
                      tname, value.type_name(), value.type_name()));
      return false;
    }
    return true;
  }

  errs.push_back(
      std::format("Type {} is not supported for JSON compatibility", tname));
  return false;
}

void set_scalar_numeric(const msce::Type &type, void *target, json value)
{
  if (type.is_integer())
  {
    if (type.is_unsigned())
    {
      switch (type.get_size())
      {
      case 1:
        *reinterpret_cast<uint8_t *>(target) =
            static_cast<uint8_t>(value.get<uint64_t>());
        break;
      case 2:
        *reinterpret_cast<uint16_t *>(target) =
            static_cast<uint16_t>(value.get<uint64_t>());
        break;
      case 4:
        *reinterpret_cast<uint32_t *>(target) =
            static_cast<uint32_t>(value.get<uint64_t>());
        break;
      case 8:
        *reinterpret_cast<uint64_t *>(target) = value.get<uint64_t>();
        break;
      default:
        break;
      }
    }
    else
    {
      switch (type.get_size())
      {
      case 1:
        *reinterpret_cast<int8_t *>(target) =
            static_cast<int8_t>(value.get<int64_t>());
        break;
      case 2:
        *reinterpret_cast<int16_t *>(target) =
            static_cast<int16_t>(value.get<int64_t>());
        break;
      case 4:
        *reinterpret_cast<int32_t *>(target) =
            static_cast<int32_t>(value.get<int64_t>());
        break;
      case 8:
        *reinterpret_cast<int64_t *>(target) = value.get<int64_t>();
        break;
      default:
        break;
      }
    }
  }
  else if (type.is_floating_point())
  {
    switch (type.get_size())
    {
    case 4:
      *reinterpret_cast<float *>(target) = value.get<float>();
      break;
    case 8:
      *reinterpret_cast<double *>(target) = value.get<double>();
      break;
    case 16:
      *reinterpret_cast<long double *>(target) = value.get<long double>();
      break;
    default:
      break;
    }
  }
}

void set_vector_from_json(const msce::Type &type, void *target, json value)
{
  const std::string tname = type.get_name_str();
  std::string elem_name = tname.substr(tname.find('<') + 1);
  if (!elem_name.empty() && elem_name.back() == '>') elem_name.pop_back();
  while (!elem_name.empty() && std::isspace((unsigned char)elem_name.front()))
    elem_name.erase(elem_name.begin());
  while (!elem_name.empty() && std::isspace((unsigned char)elem_name.back()))
    elem_name.pop_back();

  if (elem_name == "bool")
  {
    std::vector<bool> v;
    v.reserve(value.size());
    for (auto &el : value)
      v.push_back(el.get<bool>());
    *reinterpret_cast<std::vector<bool> *>(target) = std::move(v);
  }
  else if (elem_name == "char")
  {
    std::vector<char> v;
    v.reserve(value.size());
    for (auto &el : value)
      v.push_back(static_cast<char>(el.get<int>()));
    *reinterpret_cast<std::vector<char> *>(target) = std::move(v);
  }
  else if (elem_name == "unsigned char")
  {
    std::vector<unsigned char> v;
    v.reserve(value.size());
    for (auto &el : value)
      v.push_back(static_cast<unsigned char>(el.get<uint64_t>()));
    *reinterpret_cast<std::vector<unsigned char> *>(target) = std::move(v);
  }
  else if (elem_name == "char8_t")
  {
    std::vector<char8_t> v;
    v.reserve(value.size());
    for (auto &el : value)
      v.push_back(static_cast<char8_t>(el.get<uint64_t>()));
    *reinterpret_cast<std::vector<char8_t> *>(target) = std::move(v);
  }
  else if (elem_name == "char16_t")
  {
    std::vector<char16_t> v;
    v.reserve(value.size());
    for (auto &el : value)
      v.push_back(static_cast<char16_t>(el.get<uint64_t>()));
    *reinterpret_cast<std::vector<char16_t> *>(target) = std::move(v);
  }
  else if (elem_name == "char32_t")
  {
    std::vector<char32_t> v;
    v.reserve(value.size());
    for (auto &el : value)
      v.push_back(static_cast<char32_t>(el.get<uint64_t>()));
    *reinterpret_cast<std::vector<char32_t> *>(target) = std::move(v);
  }
  else if (elem_name == "short")
  {
    std::vector<short> v;
    v.reserve(value.size());
    for (auto &el : value)
      v.push_back(static_cast<short>(el.get<int64_t>()));
    *reinterpret_cast<std::vector<short> *>(target) = std::move(v);
  }
  else if (elem_name == "unsigned short")
  {
    std::vector<unsigned short> v;
    v.reserve(value.size());
    for (auto &el : value)
      v.push_back(static_cast<unsigned short>(el.get<uint64_t>()));
    *reinterpret_cast<std::vector<unsigned short> *>(target) = std::move(v);
  }
  else if (elem_name == "int")
  {
    std::vector<int> v;
    v.reserve(value.size());
    for (auto &el : value)
      v.push_back(el.get<int>());
    *reinterpret_cast<std::vector<int> *>(target) = std::move(v);
  }
  else if (elem_name == "unsigned int")
  {
    std::vector<unsigned int> v;
    v.reserve(value.size());
    for (auto &el : value)
      v.push_back(static_cast<unsigned int>(el.get<uint64_t>()));
    *reinterpret_cast<std::vector<unsigned int> *>(target) = std::move(v);
  }
  else if (elem_name == "long")
  {
    std::vector<long> v;
    v.reserve(value.size());
    for (auto &el : value)
      v.push_back(static_cast<long>(el.get<int64_t>()));
    *reinterpret_cast<std::vector<long> *>(target) = std::move(v);
  }
  else if (elem_name == "unsigned long")
  {
    std::vector<unsigned long> v;
    v.reserve(value.size());
    for (auto &el : value)
      v.push_back(static_cast<unsigned long>(el.get<uint64_t>()));
    *reinterpret_cast<std::vector<unsigned long> *>(target) = std::move(v);
  }
  else if (elem_name == "long long")
  {
    std::vector<long long> v;
    v.reserve(value.size());
    for (auto &el : value)
      v.push_back(el.get<int64_t>());
    *reinterpret_cast<std::vector<long long> *>(target) = std::move(v);
  }
  else if (elem_name == "unsigned long long")
  {
    std::vector<unsigned long long> v;
    v.reserve(value.size());
    for (auto &el : value)
      v.push_back(el.get<uint64_t>());
    *reinterpret_cast<std::vector<unsigned long long> *>(target) = std::move(v);
  }
  else if (elem_name == "float")
  {
    std::vector<float> v;
    v.reserve(value.size());
    for (auto &el : value)
      v.push_back(el.get<float>());
    *reinterpret_cast<std::vector<float> *>(target) = std::move(v);
  }
  else if (elem_name == "double")
  {
    std::vector<double> v;
    v.reserve(value.size());
    for (auto &el : value)
      v.push_back(el.get<double>());
    *reinterpret_cast<std::vector<double> *>(target) = std::move(v);
  }
  else if (elem_name == "long double")
  {
    std::vector<long double> v;
    v.reserve(value.size());
    for (auto &el : value)
      v.push_back(el.get<long double>());
    *reinterpret_cast<std::vector<long double> *>(target) = std::move(v);
  }
  else if (elem_name == "std::string")
  {
    std::vector<std::string> v;
    v.reserve(value.size());
    for (auto &el : value)
      v.push_back(el.get<std::string>());
    *reinterpret_cast<std::vector<std::string> *>(target) = std::move(v);
  }
  else
  {
    msce::Logger("Server").log_warning(
        "Vector element type '{}' is not handled by set_vector_from_json",
        elem_name);
  }
}

void set_from_json(const msce::Type &type, void *target, json value)
{
  if (!target) return;
  const std::string tname = type.get_name_str();

  if (type.get_std_type() == typeid(bool))
  {
    *reinterpret_cast<bool *>(target) = value.get<bool>();
    return;
  }

  if (type.get_std_type() == typeid(std::string))
  {
    *reinterpret_cast<std::string *>(target) = value.get<std::string>();
    return;
  }

  if (type.is_numeric())
  {
    set_scalar_numeric(type, target, value);
    return;
  }

  if (type.is_enum())
  {
    if (value.is_number_integer())
    {
      if (type.is_unsigned())
      {
        switch (type.get_size())
        {
        case 1:
          *reinterpret_cast<uint8_t *>(target) =
              static_cast<uint8_t>(value.get<uint64_t>());
          break;
        case 2:
          *reinterpret_cast<uint16_t *>(target) =
              static_cast<uint16_t>(value.get<uint64_t>());
          break;
        case 4:
          *reinterpret_cast<uint32_t *>(target) =
              static_cast<uint32_t>(value.get<uint64_t>());
          break;
        case 8:
          *reinterpret_cast<uint64_t *>(target) = value.get<uint64_t>();
          break;
        default:
          break;
        }
      }
      else
      {
        switch (type.get_size())
        {
        case 1:
          *reinterpret_cast<int8_t *>(target) =
              static_cast<int8_t>(value.get<int64_t>());
          break;
        case 2:
          *reinterpret_cast<int16_t *>(target) =
              static_cast<int16_t>(value.get<int64_t>());
          break;
        case 4:
          *reinterpret_cast<int32_t *>(target) =
              static_cast<int32_t>(value.get<int64_t>());
          break;
        case 8:
          *reinterpret_cast<int64_t *>(target) = value.get<int64_t>();
          break;
        default:
          break;
        }
      }
    }
    return;
  }

  if (tname.rfind("std::vector<", 0) == 0)
  {
    set_vector_from_json(type, target, value);
    return;
  }

  if (type == typeof(msce::internal::TypeComponentMap))
  {
    auto &m = *reinterpret_cast<msce::internal::TypeComponentMap *>(target);
    m.clear();
    for (auto &[key, val] : value.items())
    {
      msce::Type const *ctype = nullptr;
      try
      {
        ctype = &msce::get_reflection_of_type(key);
      }
      catch (...)
      {
        msce::Logger("Server").log_warning("Unknown component type '{}'", key);
        continue;
      }
      auto handle = msce::ComponentManager::instance->create_component(*ctype);
      msce::IComponent *comp = handle.get();
      if (!comp) continue;
      set_from_json(*ctype, comp, val);
      m[ctype->get_std_type()] = handle;
    }
    return;
  }

  if (type.is_class())
  {
    for (auto &[key, val] : value.items())
    {
      if (!type.has_member_named(key.c_str())) continue;
      const msce::Type &mtype = type.get_member_named(key.c_str()).get_type();
      void *mptr = type.get_member_named(key.c_str()).get_ptr(target, type);
      if (!mptr) continue;
      set_from_json(mtype, mptr, val);
    }
    return;
  }
}

// ---------- router handlers ----------

void get_type_fields(const httplib::Request &req, httplib::Response &res)
{
  std::stringstream ss(req.body);
  cereal::JSONInputArchive ar(ss);
  GetTypeFieldsBody data;
  data.load(ar);

  std::vector<FieldInfo> fields;

  for (const msce::MemberInfo &m : typeof_n(data.type).get_members())
    fields.push_back(FieldInfo{m.get_name_str(), m.get_type().get_name_str(),
                               m.get_type().is_class(),
                               m.get_type().is_enum()});

  std::stringstream oss;

  {
    cereal::JSONOutputArchive oar(oss);
    oar(cereal::make_nvp("fields", fields));
  }

  res.set_content(oss.str(), "application/json");
}

void get_types(const httplib::Request &, httplib::Response &res)
{
  std::vector<std::string> names;
  for (std::string_view v : msce::Type::get_all_type_names())
    names.emplace_back(
        base_type_name(msce::get_reflection_of_type(std::string(v))));

  std::stringstream ss;
  {
    cereal::JSONOutputArchive ar(ss);
    ar(cereal::make_nvp("typenames", names));
  }
  res.set_content(ss.str(), "application/json");
}

void handle_css(const httplib::Request &req, httplib::Response &res)
{
  res.set_file_content("." + req.path, "text/css");
}

void handle_js(const httplib::Request &req, httplib::Response &res)
{
  res.set_file_content("." + req.path, "text/javascript");
}

void handle_main_page(const httplib::Request &req, httplib::Response &res)
{
  res.set_file_content(
      "/home/carry/Code/MSCE/msce_editor_thingie/frontend/dist/index.html",
      "text/html");
}

void log_message(const httplib::Request &req, httplib::Response &res)
{
  static msce::Logger client_log("Client");
  std::stringstream ss(req.body);
  cereal::JSONInputArchive ar(ss);
  LogMessagePayload p;

  p.load(ar);
  client_log.log_info(p.message);
}

void get_prototype_names(const httplib::Request &req, httplib::Response &res)
{
  std::vector<std::string> names;
  for (auto t : msce::PrototypeManager::instance->get_prototype_types())
    names.emplace_back(t.get().get_name());

  std::stringstream ss;
  {
    cereal::JSONOutputArchive ar(ss);
    ar(cereal::make_nvp("typenames", names));
  }
  res.set_content(ss.str(), "application/json");
}

void create_prototype(const httplib::Request &req, httplib::Response &res)
{
  auto j = json::parse(req.body);
  std::string id = j["fields"].contains("id") ? j["fields"]["id"]
                                              : "PLACEHOLDER_ID CHANGE ME!!!!";
  if (msce::PrototypeManager::instance->prototype_id_exists(id))
  {
    auto e = json();
    e["errors"] = std::vector<std::string>{"Prototype with id '" + id +
                                           "' already exists."};
    res.set_content(e.dump(), "application/json");
    return;
  }

  msce::IPrototype *p = nullptr;

  {
    std::string type = j["type"];

    p = msce::PrototypeManager::instance->create_new_prototype_instance(type,
                                                                        id);
  }

  const msce::Type &type = typeof_n(j["type"]);

  std::vector<std::string> errs;

  if (j.contains("fields") && j["fields"].is_object())
  {
    for (auto &[key, value] : j["fields"].items())
    {
      const char *kstr = key.c_str();
      if (!type.has_member_named(kstr))
      {
        errs.push_back(std::format("Field {} doesn't exist.", kstr));
        continue;
      }

      const msce::Type &mtype = type.get_member_named(kstr).get_type();
      void *mptr = type.get_member_named(kstr).get_ptr(p, type);

      if (!mptr)
      {
        errs.push_back(std::format("Failed to access field {}.", kstr));
        continue;
      }

      if (!types_compatible(mtype, value, errs))
      {
        logger.log_warning(
            "Failed to set {}. Its type({}) is incompatible with {}", kstr,
            mtype.get_name(), value.type_name());
        continue;
      }

      set_from_json(mtype, mptr, value);
    }
  }

  json res_body;
  res_body["errors"] = errs;
  res.set_content(res_body.dump(), "application/json");
  msce::PrototypeManager::instance->serialize_prototype(
      std::format(".msce/prototypes/{}.msceproto", id), id);
  msce::PrototypeManager::instance->deserialize_prototype(
      std::format(".msce/prototypes/{}.msceproto", id));
}

void get_prototype_ids(const httplib::Request &req, httplib::Response &res)
{
  std::vector<std::string> ids;
  for (auto *proto : msce::PrototypeManager::instance->enumerate_prototypes())
  {
    if (!proto) continue;
    ids.push_back(proto->id);
  }

  auto j = json();
  j["ids"] = ids;
  res.set_content(j.dump(), "application/json");
}

inline std::string base_type_name(const msce::Type &type);

void edit_prototype(const httplib::Request &req, httplib::Response &res)
{
  auto j = json::parse(req.body);
  std::string id = j.contains("id") && j["id"].is_string()
                       ? j["id"].get<std::string>()
                       : "PLACEHOLDER_ID CHANGE ME!!!!";

  auto *existing = msce::PrototypeManager::instance->get_prototype(id);
  if (!existing)
  {
    auto e = json();
    e["errors"] = std::vector<std::string>{"Prototype with id '" + id +
                                           "' does not exist."};
    res.set_content(e.dump(), "application/json");
    return;
  }

  std::string type =
      j.contains("type") && j["type"].is_string()
          ? j["type"].get<std::string>()
          : base_type_name(existing->get_type_info_polymorphic());

  // remove the old instance before recreating it
  msce::PrototypeManager::instance->delete_prototype(id);

  // rebuild the request body so create_prototype() can consume it unchanged
  json create_body;
  create_body["type"] = type;
  create_body["fields"] = j.contains("fields") && j["fields"].is_object()
                              ? j["fields"]
                              : json::object();
  if (!create_body["fields"].contains("id")) create_body["fields"]["id"] = id;

  httplib::Request create_req = req;
  create_req.body = create_body.dump();
  create_prototype(create_req, res);
}

// ---------- reflection -> json helper ----------

json value_to_json(const msce::Type &type, void *ptr)
{
  if (!ptr) return nullptr;

  const std::string tname = type.get_name_str();

  if (type.get_std_type() == typeid(bool))
    return *reinterpret_cast<bool *>(ptr);

  if (type.get_std_type() == typeid(std::string))
    return *reinterpret_cast<std::string *>(ptr);

  if (type.is_numeric())
  {
    if (type.is_integer())
    {
      if (type.is_unsigned())
      {
        switch (type.get_size())
        {
        case 1:
          return *reinterpret_cast<uint8_t *>(ptr);
        case 2:
          return *reinterpret_cast<uint16_t *>(ptr);
        case 4:
          return *reinterpret_cast<uint32_t *>(ptr);
        case 8:
          return *reinterpret_cast<uint64_t *>(ptr);
        default:
          return nullptr;
        }
      }
      else
      {
        switch (type.get_size())
        {
        case 1:
          return *reinterpret_cast<int8_t *>(ptr);
        case 2:
          return *reinterpret_cast<int16_t *>(ptr);
        case 4:
          return *reinterpret_cast<int32_t *>(ptr);
        case 8:
          return *reinterpret_cast<int64_t *>(ptr);
        default:
          return nullptr;
        }
      }
    }
    else if (type.is_floating_point())
    {
      switch (type.get_size())
      {
      case 4:
        return *reinterpret_cast<float *>(ptr);
      case 8:
        return *reinterpret_cast<double *>(ptr);
      case 16:
        return *reinterpret_cast<long double *>(ptr);
      default:
        return nullptr;
      }
    }
    return nullptr;
  }

  if (type.is_enum())
  {
    // return the underlying integer value
    if (type.is_unsigned())
    {
      switch (type.get_size())
      {
      case 1:
        return *reinterpret_cast<uint8_t *>(ptr);
      case 2:
        return *reinterpret_cast<uint16_t *>(ptr);
      case 4:
        return *reinterpret_cast<uint32_t *>(ptr);
      case 8:
        return *reinterpret_cast<uint64_t *>(ptr);
      default:
        return nullptr;
      }
    }
    else
    {
      switch (type.get_size())
      {
      case 1:
        return *reinterpret_cast<int8_t *>(ptr);
      case 2:
        return *reinterpret_cast<int16_t *>(ptr);
      case 4:
        return *reinterpret_cast<int32_t *>(ptr);
      case 8:
        return *reinterpret_cast<int64_t *>(ptr);
      default:
        return nullptr;
      }
    }
  }

  if (tname.rfind("std::vector<", 0) == 0)
  {
    std::string elem_name = tname.substr(tname.find('<') + 1);
    if (!elem_name.empty() && elem_name.back() == '>') elem_name.pop_back();
    while (!elem_name.empty() && std::isspace((unsigned char)elem_name.front()))
      elem_name.erase(elem_name.begin());
    while (!elem_name.empty() && std::isspace((unsigned char)elem_name.back()))
      elem_name.pop_back();

    if (elem_name == "bool")
    {
      const auto &v = *reinterpret_cast<const std::vector<bool> *>(ptr);
      json arr = json::array();
      for (bool x : v)
        arr.push_back(x);
      return arr;
    }
    if (elem_name == "char")
    {
      const auto &v = *reinterpret_cast<const std::vector<char> *>(ptr);
      json arr = json::array();
      for (char x : v)
        arr.push_back(x);
      return arr;
    }
    if (elem_name == "unsigned char")
    {
      const auto &v =
          *reinterpret_cast<const std::vector<unsigned char> *>(ptr);
      json arr = json::array();
      for (unsigned char x : v)
        arr.push_back(x);
      return arr;
    }
    if (elem_name == "short")
    {
      const auto &v = *reinterpret_cast<const std::vector<short> *>(ptr);
      json arr = json::array();
      for (short x : v)
        arr.push_back(x);
      return arr;
    }
    if (elem_name == "unsigned short")
    {
      const auto &v =
          *reinterpret_cast<const std::vector<unsigned short> *>(ptr);
      json arr = json::array();
      for (unsigned short x : v)
        arr.push_back(x);
      return arr;
    }
    if (elem_name == "int")
    {
      const auto &v = *reinterpret_cast<const std::vector<int> *>(ptr);
      json arr = json::array();
      for (int x : v)
        arr.push_back(x);
      return arr;
    }
    if (elem_name == "unsigned int")
    {
      const auto &v = *reinterpret_cast<const std::vector<unsigned int> *>(ptr);
      json arr = json::array();
      for (unsigned int x : v)
        arr.push_back(x);
      return arr;
    }
    if (elem_name == "long")
    {
      const auto &v = *reinterpret_cast<const std::vector<long> *>(ptr);
      json arr = json::array();
      for (long x : v)
        arr.push_back(x);
      return arr;
    }
    if (elem_name == "unsigned long")
    {
      const auto &v =
          *reinterpret_cast<const std::vector<unsigned long> *>(ptr);
      json arr = json::array();
      for (unsigned long x : v)
        arr.push_back(x);
      return arr;
    }
    if (elem_name == "long long")
    {
      const auto &v = *reinterpret_cast<const std::vector<long long> *>(ptr);
      json arr = json::array();
      for (long long x : v)
        arr.push_back(x);
      return arr;
    }
    if (elem_name == "unsigned long long")
    {
      const auto &v =
          *reinterpret_cast<const std::vector<unsigned long long> *>(ptr);
      json arr = json::array();
      for (unsigned long long x : v)
        arr.push_back(x);
      return arr;
    }
    if (elem_name == "float")
    {
      const auto &v = *reinterpret_cast<const std::vector<float> *>(ptr);
      json arr = json::array();
      for (float x : v)
        arr.push_back(x);
      return arr;
    }
    if (elem_name == "double")
    {
      const auto &v = *reinterpret_cast<const std::vector<double> *>(ptr);
      json arr = json::array();
      for (double x : v)
        arr.push_back(x);
      return arr;
    }
    if (elem_name == "long double")
    {
      const auto &v = *reinterpret_cast<const std::vector<long double> *>(ptr);
      json arr = json::array();
      for (long double x : v)
        arr.push_back(x);
      return arr;
    }
    if (elem_name == "std::string")
    {
      const auto &v = *reinterpret_cast<const std::vector<std::string> *>(ptr);
      json arr = json::array();
      for (const std::string &x : v)
        arr.push_back(x);
      return arr;
    }

    // class element types: attempt via reflection (size unknown -> best effort
    // empty)
    try
    {
      const msce::Type &elem_type = msce::get_reflection_of_type(elem_name);
      if (elem_type.is_class())
      {
        // Without the concrete C++ vector type we cannot safely read its size;
        // leave unimplemented for non-primitive element types.
        msce::Logger("Server").log_warning(
            "Vector element type '{}' is not handled by value_to_json",
            elem_name);
      }
    }
    catch (...)
    {
    }
    return json::array();
  }

  if (type == typeof(msce::internal::TypeComponentMap))
  {
    const auto &m =
        *reinterpret_cast<const msce::internal::TypeComponentMap *>(ptr);
    json obj = json::object();
    for (const auto &[ti, comp] : m)
    {
      if (!comp) continue;
      const msce::Type &ctype = comp->polymorphic_type();
      obj[ctype.get_name_str()] = value_to_json(ctype, comp.get());
    }
    return obj;
  }

  if (type.is_class())
  {
    json obj = json::object();
    for (const msce::MemberInfo &m : type.get_members())
    {
      void *mptr = m.get_ptr(ptr, type);
      obj[m.get_name()] = value_to_json(m.get_type(), mptr);
    }
    return obj;
  }

  return nullptr;
}

inline std::string base_type_name(const msce::Type &type)
{
  std::string name = type.get_name_str();
  // strip cv/ref/pointer qualifiers to get a registered factory name
  // e.g. "const TestPrototype &" -> "TestPrototype"
  size_t pos = 0;
  while ((pos = name.find("const ", pos)) != std::string::npos)
    name.erase(pos, 6);
  // remove trailing " &" / " *" and standalone qualifiers
  while (!name.empty() &&
         (name.back() == '&' || name.back() == '*' || name.back() == ' '))
    name.pop_back();
  // collapse multiple spaces
  std::string cleaned;
  bool space = false;
  for (char c : name)
  {
    if (c == ' ')
    {
      if (!space) cleaned.push_back(c);
      space = true;
    }
    else
    {
      cleaned.push_back(c);
      space = false;
    }
  }
  // trim
  while (!cleaned.empty() && cleaned.front() == ' ')
    cleaned.erase(cleaned.begin());
  while (!cleaned.empty() && cleaned.back() == ' ')
    cleaned.pop_back();
  return cleaned.empty() ? type.get_name_str() : cleaned;
}
void get_prototype_fields(const httplib::Request &req, httplib::Response &res)
{
  json j;
  try
  {
    j = json::parse(req.body);
  }
  catch (...)
  {
    res.status = 400;
    res.set_content(
        json{{"errors", std::vector<std::string>{"Invalid JSON body."}}}.dump(),
        "application/json");
    return;
  }

  if (!j.contains("id") || !j["id"].is_string())
  {
    res.status = 400;
    res.set_content(
        json{{"errors", std::vector<std::string>{"Missing 'id'."}}}.dump(),
        "application/json");
    return;
  }

  std::string id = j["id"];
  auto *p = msce::PrototypeManager::instance->get_prototype(id);
  if (!p)
  {
    res.status = 404;
    res.set_content(
        json{{"errors", std::vector<std::string>{"Prototype not found: " + id}}}
            .dump(),
        "application/json");
    return;
  }

  const msce::Type &type = p->get_type_info_polymorphic();

  json jout = json::object();
  jout["id"] = id;
  jout["type"] = base_type_name(type);

  json fields = json::object();
  for (const msce::MemberInfo &member : type.get_members())
  {
    void *mptr = member.get_ptr(p, type);
    fields[member.get_name()] = value_to_json(member.get_type(), mptr);
  }
  jout["fields"] = fields;

  res.set_content(jout.dump(), "application/json");
}
//   res.set_content(jout.dump(), "application/json");
// }

void get_entity_ids(const httplib::Request &req, httplib::Response &res)
{
  std::vector<uint32_t> ids;
  msce::EntityManager::instance->do_for_each_entity(
      [&](msce::EntityHandle &h)
      {
        if (h) ids.push_back(h.get_index());
      });
  json j;
  j["ids"] = ids;
  res.set_content(j.dump(), "application/json");
}

void get_component_types(const httplib::Request &req, httplib::Response &res)
{
  auto j = json();
  std::vector<std::string> types;
  for (const msce::Type &t :
       msce::ComponentManager::instance->component_types())
    types.push_back(t.get_name_str());
  j["types"] = types;
  res.set_content(j.dump(), "application/json");
}

void get_entity_fields(const httplib::Request &req, httplib::Response &res)
{
  json j;
  try
  {
    j = json::parse(req.body);
  }
  catch (...)
  {
    res.status = 400;
    res.set_content(
        json{{"errors", std::vector<std::string>{"Invalid JSON body."}}}.dump(),
        "application/json");
    return;
  }
  if (!j.contains("id"))
  {
    res.status = 400;
    res.set_content(
        json{{"errors", std::vector<std::string>{"Missing 'id'."}}}.dump(),
        "application/json");
    return;
  }
  uint32_t id = j["id"].get<uint32_t>();
  auto h = msce::EntityManager::instance->get_entity(id);
  if (!h)
  {
    res.status = 404;
    res.set_content(
        json{{"errors", std::vector<std::string>{"Entity not found."}}}.dump(),
        "application/json");
    return;
  }
  msce::Entity *e = h.get();
  const msce::Type &type = typeof(msce::Entity);
  json jout = json::object();
  jout["id"] = id;
  jout["type"] = "msce::Entity";
  json fields = json::object();
  for (const msce::MemberInfo &member : type.get_members())
  {
    void *mptr = member.get_ptr(e, type);
    fields[member.get_name()] = value_to_json(member.get_type(), mptr);
  }
  jout["fields"] = fields;
  res.set_content(jout.dump(), "application/json");
}

void create_entity(const httplib::Request &req, httplib::Response &res)
{
  auto j = json::parse(req.body);
  auto h = msce::EntityManager::instance->create_entity();
  msce::Entity *e = h.get();
  const msce::Type &type = typeof(msce::Entity);
  std::vector<std::string> errs;
  if (j.contains("fields") && j["fields"].is_object())
  {
    for (auto &[key, value] : j["fields"].items())
    {
      if (!type.has_member_named(key.c_str()))
      {
        errs.push_back(std::format("Field {} doesn't exist.", key));
        continue;
      }
      const msce::Type &mtype = type.get_member_named(key.c_str()).get_type();
      void *mptr = type.get_member_named(key.c_str()).get_ptr(e, type);
      if (!mptr)
      {
        errs.push_back(std::format("Failed to access field {}.", key));
        continue;
      }
      if (!types_compatible(mtype, value, errs)) continue;
      set_from_json(mtype, mptr, value);
    }
  }
  json res_body;
  res_body["errors"] = errs;
  res_body["id"] = h.get_index();
  res.set_content(res_body.dump(), "application/json");
}

void edit_entity(const httplib::Request &req, httplib::Response &res)
{
  auto j = json::parse(req.body);
  uint32_t id = j.contains("id") ? j["id"].get<uint32_t>() : 0;
  auto h = msce::EntityManager::instance->get_entity(id);
  if (!h)
  {
    json e;
    e["errors"] = std::vector<std::string>{"Entity does not exist."};
    res.set_content(e.dump(), "application/json");
    return;
  }
  msce::Entity *e = h.get();
  const msce::Type &type = typeof(msce::Entity);
  std::vector<std::string> errs;
  if (j.contains("fields") && j["fields"].is_object())
  {
    for (auto &[key, value] : j["fields"].items())
    {
      if (!type.has_member_named(key.c_str()))
      {
        errs.push_back(std::format("Field {} doesn't exist.", key));
        continue;
      }
      const msce::Type &mtype = type.get_member_named(key.c_str()).get_type();
      void *mptr = type.get_member_named(key.c_str()).get_ptr(e, type);
      if (!mptr)
      {
        errs.push_back(std::format("Failed to access field {}.", key));
        continue;
      }
      if (!types_compatible(mtype, value, errs)) continue;
      set_from_json(mtype, mptr, value);
    }
  }
  json res_body;
  res_body["errors"] = errs;
  res.set_content(res_body.dump(), "application/json");
}
