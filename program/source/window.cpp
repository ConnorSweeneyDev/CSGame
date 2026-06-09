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
          .width = 1280,
          .height = 720,
          .fullscreen = false,
          .vsync = true,
        },
        initial_graphics{
          .title = "CSGame",
        })
  {
  }

  void window::on_event(const SDL_Event &event)
  {
    if (event.type != SDL_EVENT_KEY_DOWN || event.key.repeat) return;
    switch (const auto &key{event.key}; key.scancode)
    {
      case SDL_SCANCODE_ESCAPE: state.active.running = false; break;
      case SDL_SCANCODE_F11: state.active.fullscreen = !state.active.fullscreen; break;
      case SDL_SCANCODE_F12: state.active.vsync = !state.active.vsync; break;
      default: break;
    }
  }

  void window::on_simulate(const double)
  {
    if (!state.previous.vsync && state.active.vsync)
      cse::print<COUT>("VSync toggled on\n");
    else if (state.previous.vsync && !state.active.vsync)
      cse::print<COUT>("VSync toggled off\n");
  }
}
