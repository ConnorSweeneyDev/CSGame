#include "object.hpp"

#include <algorithm>
#include <cstdlib>

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_scancode.h"
#include "cse/collision.hpp"
#include "cse/game.hpp"
#include "cse/name.hpp"
#include "cse/numeric.hpp"
#include "cse/object.hpp"
#include "cse/pointer.hpp"
#include "cse/resource.hpp"
#include "cse/scene.hpp"
#include "cse/window.hpp"
#include "glm/ext/vector_double3.hpp"

#include "locale.hpp"
#include "resource.hpp"

namespace csg
{
  player::player(const glm::dvec3 &translation_)
    : cse::object({.translation = {translation_},
                   .rotation = {0.0},
                   .scale = {{1.0, 1.0}},
                   .collidable = true,
                   .texture = {.source = {.image = image::redhood, .animation = animation::redhood.idle},
                               .playback = {.frame = 0, .elapsed = 0.0, .playing = true, .speed = {1.0}, .loop = true},
                               .flip = {.horizontal = false, .vertical = false},
                               .color = {.tint = {{0.5, 0.5, 0.5, 1.0}}, .alpha = {1.0}},
                               .illumination = {.show = true, .brightness = {1.0}, .penetration = {1.0}},
                               .shadow = {.show = true, .cast = true, .darkness = {1.0}, .softness = {1.0}}},
                   .text = {.content = {"[", lexeme::player, "]"},
                            .source = {.font = font::text, .animation = animation::text.main},
                            .playback = {.frame = 0, .elapsed = 0.0, .playing = false, .speed = {0.0}, .loop = false},
                            .align = {.horizontal = {.preset = CENTER, .spacing = {0.0}},
                                      .vertical = {.preset = TOP, .spacing = {0.0}},
                                      .offset = {{0.0, -5.0}}},
                            .scale = {{1.0, 1.0}},
                            .overflow = {.wrap = false, .clip = false},
                            .color = {.tint = {{0.6, 0.4, 0.3, 1.0}}, .alpha = {1.0}},
                            .illumination = {.show = true, .brightness = {0.7}, .penetration = {1.0}},
                            .shadow = {.show = false, .cast = true, .darkness = {1.0}, .softness = {0.5}}},
                   .priority = {.simulation = 0, .rendering = 1}}) {};

  void player::on_event(const SDL_Event &event)
  {
    if (event.type != SDL_EVENT_KEY_DOWN && event.type != SDL_EVENT_KEY_UP) return;
    switch (const auto &key{event.key}; key.scancode)
    {
      case SDL_SCANCODE_1:
        if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
          active.timer
            .set("texture_change",
                 [this](const bool is_redhood)
                 {
                   active.texture.playback = {};
                   if (is_redhood)
                     active.texture.source = {image::shop, animation::shop.main};
                   else
                     active.texture.source = {image::redhood, animation::redhood.idle};
                 })
            .target = 1.0;
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
          if (scene->active.objects.contains("temp"))
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
          auto &animation{active.texture.source.animation};
          if (animation == animation::redhood.idle)
          {
            animation = animation::redhood.jump;
            playback = {.loop = false};
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
      auto &component{velocity[index]};
      const auto drag{(std::abs(component) * (1.0 - (friction / max_velocity))) + friction};
      if (component > 0.0)
        component = std::max(0.0, component - (drag * tick));
      else if (component < -0.0)
        component = std::min(0.0, component + (drag * tick));
      else
        component = 0.0;
    }
    position += velocity * tick;

    auto &transparency_value{active.texture.color.alpha.value};
    auto &transparency_rate{active.texture.color.alpha.rate};
    if (keyboard[SDL_SCANCODE_A]) transparency_rate -= transparency_change;
    if (keyboard[SDL_SCANCODE_G]) transparency_rate += transparency_change;
    transparency_value += transparency_rate * tick;
    transparency_rate = 0.0;
    transparency_value = std::min(std::max(transparency_value, 0.0), 1.0);

    active.timer.call<void(const bool)>("texture_change", active.texture.source.image == image::redhood);

    auto &animation{active.texture.source.animation};
    auto &playback{active.texture.playback};
    auto final{animation.frames.size() - 1};
    if (animation == animation::redhood.jump)
      if (playback.frame == final && playback.elapsed >= animation.frames[final].duration)
      {
        animation = animation::redhood.idle;
        playback = {.speed = {2.0}};
      }
    if (previous.texture.source.animation == animation && animation == animation::redhood.idle)
      if (playback.frame == 0 && previous.texture.playback.frame == final)
      {
        playback.speed.value = 1.0;
        if (equal(active.texture.color.tint.value.r, 0.5))
          active.texture.color.tint.value.r = 0.125;
        else
          active.texture.color.tint.value.r = 0.5;
        active.texture.color.tint.instant = true;
      }
    if (previous.texture.source.image == image::shop && active.texture.source.image != image::shop)
    {
      active.texture.color.tint.value = {0.5, 0.5, 1.0, 1.0};
      active.texture.color.tint.instant = true;
    }
  }

  environment::environment(const glm::dvec3 &translation_, const cse::image &image_, const cse::animation &animation_)
    : cse::object({.translation = {translation_},
                   .rotation = {0.0},
                   .scale = {{1.0, 1.0}},
                   .collidable = true,
                   .texture = {.source = {.image = image_, .animation = animation_},
                               .playback = {.frame = 0, .elapsed = 0.0, .playing = true, .speed = {0.0}, .loop = false},
                               .flip = {.horizontal = false, .vertical = false},
                               .color = {.tint = {{0.5, 0.5, 0.5, 1.0}}, .alpha = {1.0}},
                               .illumination = {.show = true, .brightness = {1.0}, .penetration = {1.0}},
                               .shadow = {.show = true, .cast = true, .darkness = {1.0}, .softness = {1.0}}},
                   .text = {.content = "",
                            .source = {.font = {}, .animation = {}},
                            .playback = {.frame = 0, .elapsed = 0.0, .playing = false, .speed = {0.0}, .loop = false},
                            .align = {.horizontal = {.preset = CENTER, .spacing = {0.0}},
                                      .vertical = {.preset = MIDDLE, .spacing = {0.0}},
                                      .offset = {{0.0, 0.0}}},
                            .scale = {{1.0, 1.0}},
                            .overflow = {.wrap = false, .clip = true},
                            .color = {.tint = {{0.5, 0.5, 0.5, 1.0}}, .alpha = {1.0}},
                            .illumination = {.show = true, .brightness = {1.0}, .penetration = {1.0}},
                            .shadow = {.show = true, .cast = true, .darkness = {1.0}, .softness = {1.0}}},
                   .priority = {.simulation = 1, .rendering = 0}}) {};

  void environment::on_prepare()
  {
    if (name == "shop") active.texture.shadow.cast = false;
    if (name == "background3")
    {
      active.texture.illumination.brightness.value = 0.4;
      active.texture.shadow.show = false;
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
