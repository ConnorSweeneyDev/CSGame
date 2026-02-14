#include "scene.hpp"

#include <memory>

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_scancode.h"
#include "cse/game.hpp"
#include "cse/map.hpp"
#include "cse/numeric.hpp"
#include "cse/pointer.hpp"
#include "cse/print.hpp"
#include "cse/scene.hpp"
#include "glm/ext/vector_double3.hpp"
#include "glm/ext/vector_int3.hpp"

#include "camera.hpp"
#include "object.hpp"
#include "resource.hpp"

namespace csg
{
  scene::scene() : cse::scene()
  {
    hooks.set(hook::PRE_EVENT,
              [this](const SDL_Event &event)
              {
                if (event.type != SDL_EVENT_KEY_DOWN && event.type != SDL_EVENT_KEY_UP) return;
                switch (const auto &key{event.key}; key.scancode)
                {
                  case SDL_SCANCODE_6:
                    if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN) throw_lock(state.active.parent)->current("main");
                    break;
                  case SDL_SCANCODE_7:
                    if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
                      throw_lock(state.active.parent)->current("other", other);
                    break;
                  case SDL_SCANCODE_8:
                    if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN) set<csg::camera>(glm::dvec3{0.0, 0.0, 80.0});
                    break;
                  case SDL_SCANCODE_9:
                    if (const auto &player{try_find(state.active.objects, "player")})
                    {
                      if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
                        player->graphics.active.texture.color.value = {0.25, 0.0, 0.0, 0.5};
                      else if (key.type == SDL_EVENT_KEY_UP)
                        player->graphics.active.texture.color.value = {0.5, 0.5, 0.5, 1.0};
                    }
                    break;
                  default: break;
                }
              });

    hooks.set(hook::PRE_SIMULATE,
              [this](const double)
              {
                auto game{throw_lock(state.active.parent)};
                if (game->state.previous.scene.name == "main" && game->state.active.scene.name == "other")
                  cse::print<COUT>("Scene changed from \"main\" to \"other\": {}\n",
                                   game->state.previous.scene.pointer->state.active.objects.size());
                if (!equal(state.previous.camera->state.active.translation.value.x,
                           state.active.camera->state.active.translation.value.x))
                  cse::print<COUT>("Camera moved from {} to {}\n",
                                   state.previous.camera->state.active.translation.value.x,
                                   state.active.camera->state.active.translation.value.x);
              });
  }

  void scene::main(const std::shared_ptr<scene> scene)
  {
    scene->set<csg::camera>(glm::dvec3{0.0, 0.0, 80.0})
      .set<player>("player", glm::ivec3{0, 0, 0})
      .set<environment>("floor", glm::ivec3{0, -61, 0}, image::floor, animation::floor.main)
      .set<environment>("shop", glm::ivec3{80, 24, -1}, image::shop, animation::shop.main)
      .set<environment>("background1", glm::ivec3{0, 80, -3}, image::background1, animation::background1.main)
      .set<environment>("background2", glm::ivec3{0, 80, -6}, image::background2, animation::background2.main)
      .set<environment>("background3", glm::ivec3{0, 80, -9}, image::background3, animation::background3.main);
  }

  void scene::other(const std::shared_ptr<scene> scene)
  {
    scene->set<csg::camera>(glm::dvec3{0.0, 0.0, 80.0})
      .set<player>("player", glm::ivec3{0, 0, 0})
      .set<environment>("floor", glm::ivec3{0, -61, 0}, image::floor, animation::floor.main)
      .set<environment>("background3", glm::ivec3{0, 80, -9}, image::background3, animation::background3.main);
  }
}
