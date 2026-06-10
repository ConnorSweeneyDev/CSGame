#include "window.hpp"

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_scancode.h"
#include "cse/print.hpp"
#include "cse/window.hpp"

namespace csg
{
  window::window()
    : cse::window(
        initial_state{
          .display = PRIMARY,
          .left = CENTER,
          .top = CENTER,
          .width = 1280,
          .height = 720,
        },
        initial_graphics{
          .title = "CSGame",
          .fullscreen = false,
          .vsync = true,
        })
  {
  }

  void window::on_event(const SDL_Event &event)
  {
    if (event.type != SDL_EVENT_KEY_DOWN || event.key.repeat) return;
    switch (const auto &key{event.key}; key.scancode)
    {
      case SDL_SCANCODE_ESCAPE: state.active.running = false; break;
      case SDL_SCANCODE_F11: graphics.active.fullscreen = !graphics.active.fullscreen; break;
      case SDL_SCANCODE_F12: graphics.active.vsync = !graphics.active.vsync; break;
      default: break;
    }
  }

  void window::on_simulate(const double)
  {
    if (!graphics.previous.vsync && graphics.active.vsync)
      cse::print<COUT>("VSync toggled on\n");
    else if (graphics.previous.vsync && !graphics.active.vsync)
      cse::print<COUT>("VSync toggled off\n");
  }
}
