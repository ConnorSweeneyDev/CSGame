#include "game.hpp"

#include <memory>
#include <utility>

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_scancode.h"
#include "cse/game.hpp"
#include "cse/print.hpp"
#include "cse/utility.hpp"

#include "scene.hpp"
#include "window.hpp"

namespace csg
{
  game::game() : cse::game({60.0, 144.0})
  {
    hook.set("pre_event",
             [this](const SDL_Event &event)
             {
               if (event.type != SDL_EVENT_KEY_DOWN || event.key.repeat) return;
               switch (const auto &key{event.key}; key.scancode)
               {
                 case SDL_SCANCODE_F8:
                   if (equal(graphics.active.frame_rate, 144.0))
                     graphics.active.frame_rate = 30.0;
                   else
                     graphics.active.frame_rate = 144.0;
                   break;
                 case SDL_SCANCODE_F9:
                   if (equal(state.active.poll_rate, 60.0))
                     state.active.poll_rate = 300.0;
                   else
                     state.active.poll_rate = 60.0;
                   break;
                 default: break;
               }
             });

    hook.set("pre_simulate",
             [this](const float)
             {
               if (!equal(state.previous.poll_rate, state.active.poll_rate))
                 cse::print<COUT>("Poll rate changed from {} to {}\n", state.previous.poll_rate,
                                  state.active.poll_rate);
               if (!equal(graphics.previous.frame_rate, graphics.active.frame_rate))
                 cse::print<COUT>("Frame rate changed from {} to {}\n", graphics.previous.frame_rate,
                                  graphics.active.frame_rate);
             });
  }

  void game::setup(const std::shared_ptr<game> game)
  {
    game->set_window<csg::window>()->set_current_scene("main", scene::main);
  }
}
