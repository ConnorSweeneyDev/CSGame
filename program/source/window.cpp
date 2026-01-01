#include "window.hpp"

#include <string>

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_scancode.h"
#include "cse/print.hpp"
#include "cse/window.hpp"
#include "glm/ext/vector_uint2.hpp"

namespace csg
{
  window::window(const std::string &title_, const glm::uvec2 &dimensions_)
    : cse::window(title_, dimensions_, false, true)
  {
    hook.set("event",
             [this](const SDL_Event &event)
             {
               if (event.type != SDL_EVENT_KEY_DOWN || event.key.repeat) return;
               switch (const auto &key{event.key}; key.scancode)
               {
                 case SDL_SCANCODE_ESCAPE: state.running = false; break;
                 case SDL_SCANCODE_F10: graphics.title = "New Title"; break;
                 case SDL_SCANCODE_F11: state.fullscreen = !state.fullscreen; break;
                 case SDL_SCANCODE_F12: state.vsync = !state.vsync; break;
                 default: break;
               }
             });

    hook.set("simulate",
             [this]()
             {
               if (previous.graphics.title != graphics.title)
                 cse::print<COUT>("Window title changed from \"{}\" to \"{}\"\n", previous.graphics.title,
                                  graphics.title);
             });
  }
}
