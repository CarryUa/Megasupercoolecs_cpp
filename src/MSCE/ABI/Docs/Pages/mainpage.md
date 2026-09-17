\mainpage

# MegaSuperCoolEngine

This is a learning project highly inspired by
[RobustEngine by SpaceWizard team](https://github.com/space-wizards/RobustToolbox/), **but not conveying any of it's covered
works**. Therefore MSCEngine is not required to inherrit RobustEngine's GPL-3.0 license.

# Architecture

MSCEngine implements Entity-Component-System(later refered to as ECS) architecture. In ECS architecture responsibilities are
split between:

- Components, which simply hold data within them.(see \ref msce::BaseComponent and \ref msce::ComponentManager)
- Systems, which operate on data within components.(see \ref msce::System and \ref msce::SystemManager)
- Entities, which couple components together. (see \ref msce::Entity and \ref msce::EntityManager)
