#include "window.hpp"

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_scancode.h"
#include "cse/print.hpp"
#include "cse/window.hpp"

namespace csg
{
  window::window()
    : cse::window({.title = "CSGame",
                   .display = PRIMARY,
                   .left = ORIGIN,
                   .top = ORIGIN,
                   .width = 1280,
                   .height = 720,
                   .mode = WINDOWED,
                   .vsync = true,
                   .mouse = {.visible = false, .position = {0.0, 0.0}}})
  {
  }

  void window::on_event(const SDL_Event &event)
  {
    if (event.type != SDL_EVENT_KEY_DOWN || event.key.repeat) return;
    switch (const auto &key{event.key}; key.scancode)
    {
      case SDL_SCANCODE_ESCAPE: active.running = false; break;
      case SDL_SCANCODE_F11:
        if (active.mode == WINDOWED)
          active.mode = BORDERLESS;
        else
          active.mode = WINDOWED;
        break;
      case SDL_SCANCODE_F12: active.vsync = !active.vsync; break;
      default: break;
    }
  }

  void window::on_simulate(const double)
  {
    if (!previous.vsync && active.vsync)
      cse::print<COUT>("VSync toggled on\n");
    else if (previous.vsync && !active.vsync)
      cse::print<COUT>("VSync toggled off\n");
  }
}
