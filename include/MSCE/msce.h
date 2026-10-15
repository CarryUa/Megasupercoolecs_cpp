#pragma once
#include <MSCE/terminator.h>
#include <MSCE/event.h>
#include <MSCE/system.h>
#include <MSCE/component.h>
#include <MSCE/entity.h>
#include <MSCE/prototype.h>

namespace msce
{
void init_engine()
{
  std::set_terminate(msce::internal::handle_terminate);

  static auto g_event_man = msce::EventManager();
  static auto g_sys_man = msce::SystemManager();
  static auto g_comp_man = msce::ComponentManager();
  static auto g_ent_man = msce::EntityManager();
  static auto g_proto_man = msce::PrototypeManager();
  static auto g_enum_man = msce::EnumManager();

  g_sys_man.init_all_systems();
  g_proto_man.load_all_prototypes();
}
} // namespace msce