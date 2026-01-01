#include "scene.hpp"

#include <memory>

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_scancode.h"
#include "cse/game.hpp"
#include "cse/print.hpp"
#include "cse/scene.hpp"
#include "cse/system.hpp"
#include "cse/utility.hpp"

#include "camera.hpp"
#include "object.hpp"
#include "resource.hpp"

namespace csg
{
  scene::scene() : cse::scene()
  {
    hook.set("pre_event",
             [this](const SDL_Event &event)
             {
               if (event.type != SDL_EVENT_KEY_DOWN && event.type != SDL_EVENT_KEY_UP) return;
               switch (const auto &key{event.key}; key.scancode)
               {
                 case SDL_SCANCODE_6:
                   if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN) throw_lock(parent)->set_current_scene("main");
                   break;
                 case SDL_SCANCODE_7:
                   if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
                     throw_lock(parent)->set_current_scene(
                       "other",
                       [](const std::shared_ptr<scene> other)
                       {
                         other->set_camera<csg::camera>({{0.0f, 0.0f, 80.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}});
                         other->set_object<player>("player", {{0, 0, 0}, {0, 0, 0}, {1, 1, 1}});
                         other->set_object<environment>("floor", {{0, -61, 0}, {0, 0, 0}, {1, 1, 1}},
                                                        texture::floor.image, texture::floor.main);
                       });
                   break;
                 case SDL_SCANCODE_8:
                   if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN) camera->state.translation.value.x += 1.0f;
                   break;
                 case SDL_SCANCODE_9:
                   if (const auto &player{try_at(objects, "player")})
                   {
                     if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
                       player->graphics.color = {64, 0, 0, 128};
                     else if (key.type == SDL_EVENT_KEY_UP)
                       player->graphics.color = {128, 128, 128, 255};
                   }
                   break;
                 default: break;
               }
             });

    hook.set("pre_simulate",
             [this]()
             {
               auto game{throw_lock(parent)};
               if (game->previous_scene.first == "main" && throw_id(game->current_scene, game->scenes) == "other")
                 if (cse::debug)
                   cse::print<COUT>("Scene changed from \"main\" to \"other\": {}\n",
                                    game->previous_scene.second->objects.size());
             });
  }
}
