#include "window.hpp"

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_scancode.h"
#include "cse/container.hpp"
#include "cse/game.hpp"
#include "cse/print.hpp"
#include "cse/window.hpp"

#include "state.hpp"

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
                   .mouse = {.visible = false, .position = {0.0, 0.0}}}) {};

  void window::on_prepare()
  {
    const auto &settings = find_as<csg::settings>(game->active.states, "settings");
    active.display = settings->window->display;
    active.left = settings->window->position.first;
    active.top = settings->window->position.second;
    active.width = settings->window->size.first;
    active.height = settings->window->size.second;
    active.mode = settings->window->mode;
    active.vsync = settings->window->vsync;
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

  void window::on_clean()
  {
    const auto &settings = find_as<csg::settings>(game->active.states, "settings");
    settings->window->display = active.display;
    settings->window->position = {active.left, active.top};
    settings->window->size = {active.width, active.height};
    settings->window->mode = active.mode;
    settings->window->vsync = active.vsync;
  }
}
