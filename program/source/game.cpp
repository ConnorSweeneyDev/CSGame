#include "game.hpp"

#include <memory>
#include <utility>

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_scancode.h"
#include "cse/game.hpp"
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
                   if (equal(graphics.frame_rate, 144.0))
                     graphics.frame_rate = 30.0;
                   else
                     graphics.frame_rate = 144.0;
                   break;
                 case SDL_SCANCODE_F9:
                   if (equal(state.poll_rate, 60.0))
                     state.poll_rate = 300.0;
                   else
                     state.poll_rate = 60.0;
                   break;
                 default: break;
               }
             });
  }

  void game::setup(const std::shared_ptr<game> game)
  {
    game->set_window<csg::window>()->set_scene("other", csg::scene::other)->set_current_scene("main", csg::scene::main);
  }
}
