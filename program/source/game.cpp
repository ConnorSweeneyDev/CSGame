#include "game.hpp"

#include <string>

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_scancode.h"
#include "cse/container.hpp"
#include "cse/game.hpp"
#include "cse/numeric.hpp"

#include "window.hpp"

namespace csg
{
  game::game()
    : cse::game(
        initial_state{
          .tick = 300.0,
        },
        initial_graphics{
          .frame = 144.0,
          .aspect = {.value = 16.0 / 9.0, .interpolate = true},
          .resolution = 100,
          .clear = {.value = {0.0, 0.0, 0.0}, .interpolate = true},
        },
        initial_audio{
          .master = {.value = 0.5, .interpolate = true},
          .sound = {.value = 0.5, .interpolate = true},
          .music = {.value = 0.5, .interpolate = true},
        })
  {
  }

  void game::pre_event(const SDL_Event &event)
  {
    if (event.type != SDL_EVENT_KEY_DOWN || event.key.repeat) return;
    switch (const auto &key{event.key}; key.scancode)
    {
      case SDL_SCANCODE_F7: set<csg::window>(); break;
      case SDL_SCANCODE_F8:
        if (equal(graphics.active.aspect.value, 16.0 / 9.0))
          graphics.active.aspect.value = 4.0 / 3.0;
        else
          graphics.active.aspect.value = 16.0 / 9.0;
        break;
      case SDL_SCANCODE_F9:
        if (equal(graphics.active.frame.target, 144.0))
          graphics.active.frame.target = 60.0;
        else
          graphics.active.frame.target = 144.0;
        break;
      case SDL_SCANCODE_F10:
        if (equal(state.active.tick.target, 300.0))
          state.active.tick.target = 60.0;
        else
          state.active.tick.target = 300.0;
        break;
      default: break;
    }
  }

  void game::pre_simulate(const double)
  {
    throw_find(state.active.interfaces, "tick")->graphics.active.text.content =
      "TPS:" + std::to_string(state.active.tick.count);
    throw_find(state.active.interfaces, "frame")->graphics.active.text.content =
      "FPS:" + std::to_string(graphics.active.frame.count);
  }
}
