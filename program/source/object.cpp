#include "object.hpp"

#include <algorithm>
#include <cstdlib>

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_scancode.h"
#include "cse/collision.hpp"
#include "cse/container.hpp"
#include "cse/game.hpp"
#include "cse/name.hpp"
#include "cse/numeric.hpp"
#include "cse/object.hpp"
#include "cse/pointer.hpp"
#include "cse/resource.hpp"
#include "cse/scene.hpp"
#include "cse/window.hpp"
#include "glm/ext/vector_double3.hpp"

#include "resource.hpp"

namespace csg
{
  player::player(const glm::dvec3 &translation_)
    : cse::object({.translation = {.value = translation_, .interpolate = true},
                   .rotation = {.value = 0.0, .interpolate = true},
                   .scale = {.value = {1.0, 1.0}, .interpolate = true},
                   .collidable = true,
                   .shader = {.vertex = vertex::main, .fragment = fragment::main},
                   .texture = {.image = image::redhood,
                               .animation = animation::redhood.idle,
                               .playback = {.frame = 0, .speed = {1.0}, .loop = true, .elapsed = 0.0},
                               .flip = {.horizontal = false, .vertical = false},
                               .color = {.value = {0.5, 0.5, 0.5, 1.0}, .interpolate = true},
                               .transparency = {.value = 1.0, .interpolate = true}},
                   .illumination = {.show = true, .brightness = {.value = 1.0, .interpolate = true}},
                   .shadow = {.cast = true,
                              .show = true,
                              .darkness = {.value = 1.0, .interpolate = true},
                              .softness = {.value = 1.0, .interpolate = true}},
                   .priority = {.simulation = 0, .rendering = 1}})
  {
  }

  void player::on_event(const SDL_Event &event)
  {
    if (event.type != SDL_EVENT_KEY_DOWN && event.type != SDL_EVENT_KEY_UP) return;
    switch (const auto &key{event.key}; key.scancode)
    {
      case SDL_SCANCODE_1:
        if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
        {
          if (active.texture.image == image::redhood)
            active.timer.set("texture_change", 1.0,
                             [this](const bool should)
                             {
                               if (!should) return;
                               active.texture.image = image::shop;
                               active.texture.animation = animation::shop.main;
                               active.texture.playback = {};
                             });
          else
            active.timer.set("texture_change", 1.0,
                             [this](const bool should)
                             {
                               if (!should) return;
                               active.texture.image = image::redhood;
                               active.texture.animation = animation::redhood.idle;
                               active.texture.playback = {0, {1.0}, true};
                             });
        }
        break;
      case SDL_SCANCODE_2:
        if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
          active.texture.flip.horizontal = !active.texture.flip.horizontal;
        break;
      case SDL_SCANCODE_3:
        if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
        {
          if (equal(active.texture.playback.speed.value, 1.0))
            active.texture.playback.speed.value = -1.0;
          else
            active.texture.playback.speed.value = 1.0;
        }
        break;
      case SDL_SCANCODE_4:
        if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
        {
          if (try_contains(scene->active.objects, "temp"))
            scene->remove("temp");
          else
            scene->set<environment>("temp", glm::dvec3{-80.0, 24.0, -1.0}, image::shop, animation::shop.main);
        }
        break;
      case SDL_SCANCODE_5:
        if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN) scene->remove("player");
        break;
      case SDL_SCANCODE_0:
        if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
        {
          auto &playback{active.texture.playback};
          auto &animation{active.texture.animation};
          if (animation == animation::redhood.idle)
          {
            animation = animation::redhood.jump;
            playback = {0, {1.0}, false};
          }
        }
        break;
      default: break;
    }
  }

  void player::on_simulate(const double tick)
  {
    const auto &keyboard{scene->game->active.window->active.keyboard};

    auto &position{active.translation.value};
    auto &velocity{active.translation.rate};
    auto &acceleration{active.translation.curve};
    if (keyboard[SDL_SCANCODE_E]) acceleration.y += max_velocity;
    if (keyboard[SDL_SCANCODE_D]) acceleration.y -= max_velocity;
    if (keyboard[SDL_SCANCODE_F]) acceleration.x += max_velocity;
    if (keyboard[SDL_SCANCODE_S]) acceleration.x -= max_velocity;
    if (keyboard[SDL_SCANCODE_W]) acceleration.z += max_velocity;
    if (keyboard[SDL_SCANCODE_R]) acceleration.z -= max_velocity;
    velocity += acceleration * tick;
    acceleration = {0.0, 0.0, 0.0};
    for (int index{}; index < 3; ++index)
    {
      auto drag{std::abs(velocity[index]) * (1.0 - (friction / max_velocity)) + friction};
      if (velocity[index] > 0.0)
        velocity[index] = std::max(0.0, velocity[index] - drag * tick);
      else if (velocity[index] < -0.0)
        velocity[index] = std::min(0.0, velocity[index] + drag * tick);
      else
        velocity[index] = 0.0;
    }
    position += velocity * tick;

    auto &transparency_value{active.texture.transparency.value};
    auto &transparency_rate{active.texture.transparency.rate};
    if (keyboard[SDL_SCANCODE_A]) transparency_rate -= transparency_change;
    if (keyboard[SDL_SCANCODE_G]) transparency_rate += transparency_change;
    transparency_value += transparency_rate * tick;
    transparency_rate = 0.0;
    if (transparency_value < 0.0) transparency_value = 0.0;
    if (transparency_value > 1.0) transparency_value = 1.0;

    active.timer.call<void(const bool)>("texture_change", true);

    auto &animation{active.texture.animation};
    auto &playback{active.texture.playback};
    auto final{animation.frames.size() - 1};
    if (animation == animation::redhood.jump)
      if (playback.frame == final && playback.elapsed >= animation.frames[final].duration)
      {
        animation = animation::redhood.idle;
        playback = {0, {2.0}, true};
      }
    if (previous.texture.animation == animation && animation == animation::redhood.idle)
      if (playback.frame == 0 && previous.texture.playback.frame == final)
      {
        playback.speed.value = 1.0;
        if (equal(active.texture.color.value.r, 0.5))
          active.texture.color.value.r = 0.125;
        else
          active.texture.color.value.r = 0.5;
        active.texture.color.instant = true;
      }
    if (previous.texture.image == image::shop && active.texture.image != image::shop)
    {
      active.texture.color.value = {0.5, 0.5, 1.0, 1.0};
      active.texture.color.instant = true;
    }
  }

  environment::environment(const glm::dvec3 &translation_, const cse::image &image_, const cse::animation &animation_)
    : cse::object({.translation = {.value = translation_, .interpolate = true},
                   .rotation = {.value = 0.0, .interpolate = true},
                   .scale = {.value = {1.0, 1.0}, .interpolate = true},
                   .collidable = true,
                   .shader = {.vertex = vertex::main, .fragment = fragment::main},
                   .texture = {.image = image_,
                               .animation = animation_,
                               .playback = {.frame = 0, .speed = {0.0}, .loop = false, .elapsed = 0.0},
                               .flip = {.horizontal = false, .vertical = false},
                               .color = {.value = {0.5, 0.5, 0.5, 1.0}, .interpolate = true},
                               .transparency = {.value = 1.0, .interpolate = true}},
                   .illumination = {.show = true, .brightness = {.value = 1.0, .interpolate = true}},
                   .shadow = {.cast = true,
                              .show = true,
                              .darkness = {.value = 1.0, .interpolate = true},
                              .softness = {.value = 1.0, .interpolate = true}},
                   .priority = {.simulation = 1, .rendering = 0}})
  {
  }

  void environment::on_prepare()
  {
    if (name == "shop") active.shadow.cast = false;
    if (name == "background3")
    {
      active.illumination.brightness.value = 0.4;
      active.shadow.show = false;
    }
  }

  void environment::on_collide(const double)
  {
    auto &contacts{scene->active.contacts};
    for (const auto &contact : contacts)
    {
      if (name != contact.self.name) continue;
      if (contact.self.hitbox != hitbox::floor.main) continue;
      if (!is<player>(contact.target.pointer)) continue;

      auto &position{contact.target.pointer->active.translation.value};
      auto &velocity{contact.target.pointer->active.translation.rate};
      position.x += contact.penetration.x;
      position.y += contact.penetration.y;
      const auto into{(velocity.x * contact.normal.x) + (velocity.y * contact.normal.y)};
      if (into < 0.0)
      {
        velocity.x -= into * contact.normal.x;
        velocity.y -= into * contact.normal.y;
      }
      if (contact.axis == cse::axis::Y)
      {
        if (contact.normal.y > 0.0) { /* Hit from above */ }
        else if (contact.normal.y < 0.0) { /* Hit from below */ }
      }
      else if (contact.axis == cse::axis::X) { /* Hit from side */ }
    }
  }
}
