#include "window.hpp"

#include <string>

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_scancode.h"
#include "cse/window.hpp"
#include "glm/ext/vector_uint2.hpp"

namespace csg
{
  custom_window::custom_window(const std::string &title_, const glm::uvec2 &dimensions_)
    : window(title_, dimensions_, false, true)
  {
    hooks.add("event_main",
              [this](const SDL_Event &event)
              {
                if (event.type != SDL_EVENT_KEY_DOWN || event.key.repeat) return;
                switch (const auto &key{event.key}; key.scancode)
                {
                  case SDL_SCANCODE_ESCAPE: running = false; break;
                  case SDL_SCANCODE_F11: fullscreen = !fullscreen; break;
                  case SDL_SCANCODE_F12: vsync = !vsync; break;
                  default: break;
                }
              });
  }
}
