#include "game.hpp"

#include <memory>
#include <utility>

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_scancode.h"
#include "cse/game.hpp"
#include "cse/utility.hpp"

#include "camera.hpp"
#include "object.hpp"
#include "resource.hpp"
#include "scene.hpp"
#include "window.hpp"

namespace csg
{
  game::game(const std::pair<double, double> &rates_) : cse::game(rates_)
  {
    set_window<csg::window>("CSGame", {1280, 720});
    set_current_scene("main",
                      [](const std::shared_ptr<scene> main)
                      {
                        main->set_camera<camera>({{0.0f, 0.0f, 80.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}});
                        main->set_object<player>("player", {{0, 0, 0}, {0, 0, 0}, {1, 1, 1}});
                        main->set_object<environment>("floor", {{0, -61, 0}, {0, 0, 0}, {1, 1, 1}},
                                                      texture::floor.image, texture::floor.main);
                        main->set_object<environment>("shop", {{80, 24, -1}, {0, 0, 0}, {1, 1, 1}}, texture::shop.image,
                                                      texture::shop.main);
                        main->set_object<environment>("background1", {{0, 80, -3}, {0, 0, 0}, {1, 1, 1}},
                                                      texture::background1.image, texture::background1.main);
                        main->set_object<environment>("background2", {{0, 80, -6}, {0, 0, 0}, {1, 1, 1}},
                                                      texture::background2.image, texture::background2.main);
                        main->set_object<environment>("background3", {{0, 80, -9}, {0, 0, 0}, {1, 1, 1}},
                                                      texture::background3.image, texture::background3.main);
                      });

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
}
