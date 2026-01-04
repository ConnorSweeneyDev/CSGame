#include "window.hpp"

#include <string>

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_scancode.h"
#include "cse/print.hpp"
#include "cse/window.hpp"
#include "glm/ext/vector_uint2.hpp"

namespace csg
{
  window::window() : cse::window("CSGame", {1280, 720}, false, true)
  {
    hook.set("event",
             [this](const SDL_Event &event)
             {
               if (event.type != SDL_EVENT_KEY_DOWN || event.key.repeat) return;
               switch (const auto &key{event.key}; key.scancode)
               {
                 case SDL_SCANCODE_ESCAPE: state.active.running = false; break;
                 case SDL_SCANCODE_F10: graphics.active.title = "New Title"; break;
                 case SDL_SCANCODE_F11: state.active.fullscreen = !state.active.fullscreen; break;
                 case SDL_SCANCODE_F12: state.active.vsync = !state.active.vsync; break;
                 default: break;
               }
             });

    hook.set("simulate",
             [this](const float)
             {
               if (graphics.previous.title != graphics.active.title)
                 cse::print<COUT>("Window title changed from \"{}\" to \"{}\"\n", graphics.previous.title,
                                  graphics.active.title);
             });
  }
}
