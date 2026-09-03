#include "eventManager.h"

msce::EventManager::EventManager()
{
  logger.log_info("Initializing manager...");
}

void msce::EventManager::subsctibe_render_event(
    void (*callback)(ObjectBeingRenderedEvent &))
{
  this->render_events_.push_back(callback);
}

void msce::EventManager::fire_render_event(ObjectBeingRenderedEvent &event)
{
  for (auto *cb : render_events_)
  {
    cb(event);
  }
}
