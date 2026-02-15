#include "object.hpp"

#include <algorithm>
#include <cstdlib>

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_scancode.h"
#include "cse/collision.hpp"
#include "cse/name.hpp"
#include "cse/numeric.hpp"
#include "cse/object.hpp"
#include "cse/pointer.hpp"
#include "cse/resource.hpp"
#include "cse/scene.hpp"
#include "glm/ext/vector_int3.hpp"

#include "resource.hpp"

namespace csg
{
  player::player(const glm::ivec3 &translation_)
    : cse::object(
        {translation_, {0, 0, 0}, {1, 1, 1}}, {vertex::main, fragment::main},
        {image::redhood, animation::redhood.idle, {0, 1.0, true, 0.0}, {false, false}, {0.5, 0.5, 0.5, 1.0}, 1.0}, {1})
  {
    hooks.set(hook::EVENT,
              [this](const SDL_Event &event)
              {
                if (event.type != SDL_EVENT_KEY_DOWN && event.type != SDL_EVENT_KEY_UP) return;
                switch (const auto &key{event.key}; key.scancode)
                {
                  case SDL_SCANCODE_1:
                    if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
                    {
                      if (graphics.active.texture.image == image::redhood)
                        state.active.timer.set("texture_change", 1.0,
                                               [this](const bool should)
                                               {
                                                 if (!should) return;
                                                 graphics.active.texture.image = image::shop;
                                                 graphics.active.texture.animation = animation::shop.main;
                                                 graphics.active.texture.playback = {};
                                               });
                      else
                        state.active.timer.set("texture_change", 1.0,
                                               [this](const bool should)
                                               {
                                                 if (!should) return;
                                                 graphics.active.texture.image = image::redhood;
                                                 graphics.active.texture.animation = animation::redhood.idle;
                                                 graphics.active.texture.playback = {0, 1.0, true};
                                               });
                    }
                    break;
                  case SDL_SCANCODE_2:
                    if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
                      graphics.active.texture.flip.horizontal = !graphics.active.texture.flip.horizontal;
                    break;
                  case SDL_SCANCODE_3:
                    if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
                    {
                      if (equal(graphics.active.texture.playback.speed.value, 1.0))
                        graphics.active.texture.playback.speed.value = -1.0;
                      else
                        graphics.active.texture.playback.speed.value = 1.0;
                    }
                    break;
                  case SDL_SCANCODE_4:
                    if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
                    {
                      if (auto scene{throw_lock(state.active.parent)}; scene->state.active.objects.contains("temp"))
                        scene->remove("temp");
                      else
                        scene->set<environment>("temp", glm::ivec3{-80, 24, -1}, image::shop, animation::shop.main);
                    }
                    break;
                  case SDL_SCANCODE_5:
                    if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
                      throw_lock(state.active.parent)->remove("player");
                    break;
                  case SDL_SCANCODE_0:
                    if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
                    {
                      auto &playback{graphics.active.texture.playback};
                      auto &animation{graphics.active.texture.animation};
                      if (animation == animation::redhood.idle)
                      {
                        animation = animation::redhood.jump;
                        playback = {0, 1.0, false};
                      }
                    }
                    break;
                  default: break;
                }
              });

    hooks.set(hook::INPUT,
              [this](const bool *keys)
              {
                auto &acceleration{state.active.translation.curve};
                if (keys[SDL_SCANCODE_E]) acceleration.y += max_velocity;
                if (keys[SDL_SCANCODE_D]) acceleration.y -= max_velocity;
                if (keys[SDL_SCANCODE_F]) acceleration.x += max_velocity;
                if (keys[SDL_SCANCODE_S]) acceleration.x -= max_velocity;
                if (keys[SDL_SCANCODE_W]) acceleration.z += max_velocity;
                if (keys[SDL_SCANCODE_R]) acceleration.z -= max_velocity;

                auto &transparency_rate{graphics.active.texture.transparency.rate};
                if (keys[SDL_SCANCODE_A]) transparency_rate -= transparency_change;
                if (keys[SDL_SCANCODE_G]) transparency_rate += transparency_change;
              });

    hooks.set(hook::SIMULATE,
              [this](const double poll_rate)
              {
                auto &position{state.active.translation.value};
                auto &velocity{state.active.translation.rate};
                auto &acceleration{state.active.translation.curve};
                velocity += acceleration * poll_rate;
                acceleration = {0.0, 0.0, 0.0};
                for (int index{}; index < 3; ++index)
                {
                  auto drag = std::abs(velocity[index]) * (1.0 - (friction / max_velocity)) + friction;
                  if (velocity[index] > 0.0)
                    velocity[index] = std::max(0.0, velocity[index] - drag * poll_rate);
                  else if (velocity[index] < -0.0)
                    velocity[index] = std::min(0.0, velocity[index] + drag * poll_rate);
                  else
                    velocity[index] = 0.0;
                }
                position += velocity * poll_rate;

                auto &transparency_value{graphics.active.texture.transparency.value};
                auto &transparency_rate{graphics.active.texture.transparency.rate};
                transparency_value += transparency_rate * poll_rate;
                transparency_rate = 0.0;
                if (transparency_value < 0.0) transparency_value = 0.0;
                if (transparency_value > 1.0) transparency_value = 1.0;

                state.active.timer.call<void(const bool)>("texture_change", true);

                auto &animation{graphics.active.texture.animation};
                auto &playback{graphics.active.texture.playback};
                auto final{animation.frames.size() - 1};
                if (animation == animation::redhood.jump)
                  if (playback.frame == final && playback.elapsed >= animation.frames[final].duration)
                  {
                    animation = animation::redhood.idle;
                    playback = {0, 2.0, true};
                  }
                if (graphics.previous.texture.animation == animation && animation == animation::redhood.idle)
                  if (playback.frame == 0 && graphics.previous.texture.playback.frame == final)
                  {
                    playback.speed = 1.0;
                    if (equal(graphics.active.texture.color.value.r, 0.5))
                      graphics.active.texture.color.value.r = 0.125;
                    else
                      graphics.active.texture.color.value.r = 0.5;
                  }
                if (graphics.previous.texture.image == image::shop && graphics.active.texture.image != image::shop)
                  graphics.active.texture.color.value = {0.5, 0.5, 1.0, 1.0};
              });

    hooks.set(hook::COLLIDE,
              [this](const double)
              {
                auto &position = state.active.translation.value;
                auto &velocity = state.active.translation.rate;
                state.active.collision.handle(
                  [&](const cse::contact &contact)
                  {
                    if ((contact.self.hitbox != hitbox::redhood.body && contact.self.hitbox != hitbox::redhood.head) ||
                        contact.target.name != "floor" || contact.target.hitbox != hitbox::floor.main)
                      return;

                    position.x -= contact.penetration.x;
                    position.y -= contact.penetration.y;
                    const auto into = (velocity.x * contact.normal.x) + (velocity.y * contact.normal.y);
                    if (into > 0.0)
                    {
                      velocity.x -= into * contact.normal.x;
                      velocity.y -= into * contact.normal.y;
                    }
                    if (contact.minimum_axis == cse::axis::Y)
                    {
                      if (contact.normal.y < 0.0) { /* Hit floor */ }
                      else if (contact.normal.y > 0.0) { /* Hit ceiling */ }
                    }
                    else if (contact.minimum_axis == cse::axis::X) { /* Hit wall */ }
                  });
              });
  }

  environment::environment(const glm::ivec3 &translation_, const cse::image &image_, const cse::animation &animation_)
    : cse::object({translation_, {0, 0, 0}, {1, 1, 1}}, {vertex::main, fragment::main},
                  {image_, animation_, {0, 0.0, false, 0.0}, {false, false}, {0.5, 0.5, 0.5, 1.0}, 1.0}, {0})
  {
  }
}
