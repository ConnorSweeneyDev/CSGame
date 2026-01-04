#include "scene.hpp"

#include <memory>

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_scancode.h"
#include "cse/game.hpp"
#include "cse/print.hpp"
#include "cse/scene.hpp"
#include "cse/system.hpp"
#include "cse/utility.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_int3.hpp"

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
                   if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
                     throw_lock(state.active.parent)->set_current_scene("main");
                   break;
                 case SDL_SCANCODE_7:
                   if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
                     throw_lock(state.active.parent)->set_current_scene("other", other);
                   break;
                 case SDL_SCANCODE_9:
                   if (const auto &player{try_at(state.active.objects, "player")})
                   {
                     if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
                       player->graphics.active.texture.color = {64, 0, 0, 128};
                     else if (key.type == SDL_EVENT_KEY_UP)
                       player->graphics.active.texture.color = {128, 128, 128, 255};
                   }
                   break;
                 default: break;
               }
             });

    hook.set("pre_simulate",
             [this](const float)
             {
               auto game{throw_lock(state.active.parent)};
               if (game->state.previous.scene.name == "main" && game->state.active.scene.name == "other")
                 if (cse::debug)
                   cse::print<COUT>("Scene changed from \"main\" to \"other\": {}\n",
                                    game->state.previous.scene.pointer->state.active.objects.size());
             });
  }

  void scene::main(const std::shared_ptr<scene> scene)
  {
    scene->set_camera<csg::camera>(glm::vec3{0.0f, 0.0f, 80.0f})
      ->set_object<player>("player", glm::ivec3{0, 0, 0})
      ->set_object<environment>("floor", glm::ivec3{0, -61, 0}, texture::floor.image, texture::floor.main)
      ->set_object<environment>("shop", glm::ivec3{80, 24, -1}, texture::shop.image, texture::shop.main)
      ->set_object<environment>("background1", glm::ivec3{0, 80, -3}, texture::background1.image,
                                texture::background1.main)
      ->set_object<environment>("background2", glm::ivec3{0, 80, -6}, texture::background2.image,
                                texture::background2.main)
      ->set_object<environment>("background3", glm::ivec3{0, 80, -9}, texture::background3.image,
                                texture::background3.main);
  }

  void scene::other(const std::shared_ptr<scene> scene)
  {
    scene->set_camera<csg::camera>(glm::vec3{0.0f, 0.0f, 80.0f})
      ->set_object<player>("player", glm::ivec3{0, 0, 0})
      ->set_object<environment>("floor", glm::ivec3{0, -61, 0}, texture::floor.image, texture::floor.main);
  }
}
