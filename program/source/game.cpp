#include "game.hpp"

#include <string>

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_scancode.h"
#include "cse/container.hpp"
#include "cse/exception.hpp"
#include "cse/game.hpp"
#include "cse/numeric.hpp"

#include "state.hpp"
#include "window.hpp"

namespace csg
{
  game::game()
    : cse::game({.meta = {.organization = "ConnorSweeneyDev", .application = "CSGame", .version = "1.0.0"},
                 .tick = 300.0,
                 .frame = 144.0,
                 .aspect = {.value = 16.0 / 9.0, .interpolate = true},
                 .resolution = 100,
                 .clear = {.value = {0.0, 0.0, 0.0}, .interpolate = true},
                 .master = {.value = 0.5, .interpolate = true},
                 .sound = {.value = 1.0, .interpolate = true},
                 .music = {.value = 1.0, .interpolate = true}}) {};

  void game::pre_prepare()
  {
    const auto &settings{find_as<csg::settings>(active.states, "settings")};
    if (!settings->read()) throw cse::exception("Failed to read settings file");
    active.master.value = settings->game->master;
    active.sound.value = settings->game->sound;
    active.music.value = settings->game->music;
  }

  void game::pre_event(const SDL_Event &event)
  {
    if (event.type != SDL_EVENT_KEY_DOWN || event.key.repeat) return;
    switch (const auto &key{event.key}; key.scancode)
    {
      case SDL_SCANCODE_Q: active.master.value = active.master.value - 0.1; break;
      case SDL_SCANCODE_T: active.master.value = active.master.value + 0.1; break;
      case SDL_SCANCODE_F7: set<csg::window>(); break;
      case SDL_SCANCODE_F8:
        if (equal(active.aspect.value, 16.0 / 9.0))
          active.aspect.value = 4.0 / 3.0;
        else
          active.aspect.value = 16.0 / 9.0;
        break;
      case SDL_SCANCODE_F9:
        if (equal(active.frame.target, 144.0))
          active.frame.target = 60.0;
        else
          active.frame.target = 144.0;
        break;
      case SDL_SCANCODE_F10:
        if (equal(active.tick.target, 300.0))
          active.tick.target = 60.0;
        else
          active.tick.target = 300.0;
        break;
      default: break;
    }
  }

  void game::pre_simulate(const double)
  {
    find(active.interfaces, "tick")->active.text.content = "TPS:" + std::to_string(active.tick.count);
    find(active.interfaces, "frame")->active.text.content = "FPS:" + std::to_string(active.frame.count);
  }

  void game::post_clean()
  {
    const auto &settings{find_as<csg::settings>(active.states, "settings")};
    settings->game->master = active.master.value;
    settings->game->sound = active.sound.value;
    settings->game->music = active.music.value;
    if (!settings->write()) throw cse::exception("Failed to write settings file");
  }
}
