#include "scene.hpp"

#include <memory>

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_scancode.h"
#include "cse/container.hpp"
#include "cse/game.hpp"
#include "cse/numeric.hpp"
#include "cse/pointer.hpp"
#include "cse/print.hpp"
#include "cse/scene.hpp"
#include "cse/system.hpp"
#include "glm/ext/vector_double3.hpp"

#include "camera.hpp"

namespace csg
{
  scene::scene() : cse::scene() {}

  void scene::pre_event(const SDL_Event &event)
  {
    if (event.type != SDL_EVENT_KEY_DOWN && event.type != SDL_EVENT_KEY_UP) return;
    switch (const auto &key{event.key}; key.scancode)
    {
      case SDL_SCANCODE_6:
        if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN) throw_lock(state.active.parent)->current("main");
        break;
      case SDL_SCANCODE_7:
        if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN) throw_lock(state.active.parent)->current("other", other);
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
  }

  void scene::pre_simulate(const double)
  {
    if (!cse::debug) return;
    auto game{throw_lock(state.active.parent)};
    auto &current_scene{game->state.active.scene};
    auto &previous_scene{game->state.previous.scene};
    if (current_scene->state.name == "main" && previous_scene->state.name == "other")
      cse::print<COUT>("Scene changed from '{}' to '{}': {}\n", previous_scene->state.name.string(),
                       current_scene->state.name.string(), previous_scene->state.active.objects.size());
    auto &current_camera{state.active.camera};
    auto &previous_camera{state.previous.camera};
    if (!equal(previous_camera->state.active.translation.value.x, current_camera->state.active.translation.value.x))
      cse::print<COUT>("Camera moved from {} to {}\n", previous_camera->state.active.translation.value.x,
                       current_camera->state.active.translation.value.x);
  }
}
