#include "scene.hpp"

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_scancode.h"
#include "cse/scene.hpp"

namespace csg
{
  custom_scene::custom_scene() : cse::core::scene()
  {
    hooks.add("event_pre",
              [this](const SDL_Event &event)
              {
                if (event.type != SDL_EVENT_KEY_DOWN && event.type != SDL_EVENT_KEY_UP) return;
                switch (const auto &key{event.key}; key.scancode)
                {
                  case SDL_SCANCODE_8:
                    if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
                      get_camera_strict()->state.translation.value.x += 1.0f;
                    break;
                  case SDL_SCANCODE_9:
                    if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
                      get_object_strict("player")->graphics.texture.frame_group = "other";
                    else if (key.type == SDL_EVENT_KEY_UP)
                      get_object_strict("player")->graphics.texture.frame_group = "main";
                    break;
                  default: break;
                }
              });
  }
}
