#include "game.hpp"

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_scancode.h"
#include "cse/game.hpp"
#include "cse/numeric.hpp"
#include "cse/print.hpp"

#include "window.hpp"

namespace csg
{
  game::game() : cse::game(200.0, 144.0, 16.0 / 9.0, {0.0, 0.0, 0.0, 1.0}) {}

  void game::pre_event(const SDL_Event &event)
  {
    if (event.type != SDL_EVENT_KEY_DOWN || event.key.repeat) return;
    switch (const auto &key{event.key}; key.scancode)
    {
      case SDL_SCANCODE_F7: set<csg::window>(); break;
      case SDL_SCANCODE_F8:
        if (equal(graphics.active.aspect.value, 16.0 / 9.0))
          graphics.active.aspect = 4.0 / 3.0;
        else
          graphics.active.aspect = 16.0 / 9.0;
        break;
      case SDL_SCANCODE_F9:
        if (equal(graphics.active.frame, 144.0))
          graphics.active.frame = 30.0;
        else
          graphics.active.frame = 144.0;
        break;
      case SDL_SCANCODE_F10:
        if (equal(state.active.tick, 300.0))
          state.active.tick = 60.0;
        else
          state.active.tick = 300.0;
        break;
      default: break;
    }
  }

  void game::pre_simulate(const double)
  {
    if (state.previous.window != state.active.window) cse::print<COUT>("Window changed\n");
    if (equal(graphics.previous.aspect.value, 16.0 / 9.0) && equal(graphics.active.aspect.value, 4.0 / 3.0))
      cse::print<COUT>("Aspect ratio changed from 16:9 to 4:3\n");
    else if (equal(graphics.previous.aspect.value, 4.0 / 3.0) && equal(graphics.active.aspect.value, 16.0 / 9.0))
      cse::print<COUT>("Aspect ratio changed from 4:3 to 16:9\n");
    if (!equal(graphics.previous.frame, graphics.active.frame))
      cse::print<COUT>("Frame rate changed from {} to {}\n", graphics.previous.frame, graphics.active.frame);
    if (!equal(state.previous.tick, state.active.tick))
      cse::print<COUT>("Poll rate changed from {} to {}\n", state.previous.tick, state.active.tick);
  }
}
