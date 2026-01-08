#include "game.hpp"

#include <memory>

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_scancode.h"
#include "cse/game.hpp"
#include "cse/print.hpp"
#include "cse/utility.hpp"

#include "scene.hpp"
#include "window.hpp"

namespace csg
{
  game::game() : cse::game(300.0, 144.0, 16.0 / 9.0)
  {
    hook.set(hooks::PRE_EVENT,
             [this](const SDL_Event &event)
             {
               if (event.type != SDL_EVENT_KEY_DOWN || event.key.repeat) return;
               switch (const auto &key{event.key}; key.scancode)
               {
                 case SDL_SCANCODE_F7: set_window<csg::window>(); break;
                 case SDL_SCANCODE_F8:
                   if (equal(graphics.active.aspect_ratio, 16.0 / 9.0))
                     graphics.active.aspect_ratio = 4.0 / 3.0;
                   else
                     graphics.active.aspect_ratio = 16.0 / 9.0;
                   break;
                 case SDL_SCANCODE_F9:
                   if (equal(graphics.active.frame_rate, 144.0))
                     graphics.active.frame_rate = 30.0;
                   else
                     graphics.active.frame_rate = 144.0;
                   break;
                 case SDL_SCANCODE_F10:
                   if (equal(state.active.poll_rate, 300.0))
                     state.active.poll_rate = 60.0;
                   else
                     state.active.poll_rate = 300.0;
                   break;
                 default: break;
               }
             });

    hook.set(
      hooks::PRE_SIMULATE,
      [this](const float)
      {
        if (state.previous.window != state.active.window) cse::print<COUT>("Window changed\n");
        if (equal(graphics.previous.aspect_ratio, 16.0 / 9.0) && equal(graphics.active.aspect_ratio, 4.0 / 3.0))
          cse::print<COUT>("Aspect ratio changed from 16:9 to 4:3\n");
        else if (equal(graphics.previous.aspect_ratio, 4.0 / 3.0) && equal(graphics.active.aspect_ratio, 16.0 / 9.0))
          cse::print<COUT>("Aspect ratio changed from 4:3 to 16:9\n");
        if (!equal(graphics.previous.frame_rate, graphics.active.frame_rate))
          cse::print<COUT>("Frame rate changed from {} to {}\n", graphics.previous.frame_rate,
                           graphics.active.frame_rate);
        if (!equal(state.previous.poll_rate, state.active.poll_rate))
          cse::print<COUT>("Poll rate changed from {} to {}\n", state.previous.poll_rate, state.active.poll_rate);
      });
  }

  void game::setup(const std::shared_ptr<game> game)
  {
    game->set_window<csg::window>()->set_current_scene("main", scene::main);
  }
}
