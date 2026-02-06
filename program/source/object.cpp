#include "object.hpp"

#include <algorithm>
#include <cstdlib>

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_scancode.h"
#include "cse/object.hpp"
#include "cse/resource.hpp"
#include "cse/scene.hpp"
#include "cse/utility.hpp"
#include "glm/ext/vector_int3.hpp"

#include "resource.hpp"

namespace csg
{
  player::player(const glm::ivec3 &translation_)
    : cse::object(
        {translation_, {0, 0, 0}, {1, 1, 1}}, {vertex::main, fragment::main},
        {texture::redhood.image, texture::redhood.idle, {0, 1.0, true, 0.0}, {false, false}, {128, 128, 128, 255}, 1.0},
        {1})
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
                      if (graphics.active.texture.image == texture::redhood.image)
                        timers.set("texture_change", 1.0,
                                   [this](const bool should)
                                   {
                                     if (!should) return;
                                     graphics.active.texture.image = texture::shop.image;
                                     graphics.active.texture.group = texture::shop.main;
                                     graphics.active.texture.animation = {};
                                   });
                      else
                        timers.set("texture_change", 1.0,
                                   [this](const bool should)
                                   {
                                     if (!should) return;
                                     graphics.active.texture.image = texture::redhood.image;
                                     graphics.active.texture.group = texture::redhood.idle;
                                     graphics.active.texture.animation = {0, 1.0, true};
                                   });
                    }
                    break;
                  case SDL_SCANCODE_2:
                    if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN) timers.remove("texture_change");
                    break;
                  case SDL_SCANCODE_3:
                    if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
                    {
                      if (equal(graphics.active.texture.animation.speed, 1.0))
                        graphics.active.texture.animation.speed = -1.0;
                      else
                        graphics.active.texture.animation.speed = 1.0;
                    }
                    break;
                  case SDL_SCANCODE_4:
                    if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
                    {
                      if (auto scene{throw_lock(state.active.parent)}; scene->state.active.objects.contains("temp"))
                        scene->remove("temp");
                      else
                        scene->set<environment>("temp", glm::ivec3{-80, 24, -1}, texture::shop.image,
                                                texture::shop.main);
                    }
                    break;
                  case SDL_SCANCODE_5:
                    if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
                      throw_lock(state.active.parent)->remove("player");
                    break;
                  case SDL_SCANCODE_0:
                    if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
                    {
                      auto &animation{graphics.active.texture.animation};
                      auto &group{graphics.active.texture.group};
                      if (group == texture::redhood.idle)
                      {
                        group = texture::redhood.jump;
                        animation = {0, 1.0, false};
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
                if (keys[SDL_SCANCODE_A]) graphics.active.texture.transparency -= 0.005;
                if (keys[SDL_SCANCODE_G]) graphics.active.texture.transparency += 0.005;
              });

    hooks.set(hook::SIMULATE,
              [this](const float poll_rate)
              {
                auto &position{state.active.translation.value};
                auto &velocity{state.active.translation.rate};
                auto &acceleration{state.active.translation.curve};
                velocity += acceleration * poll_rate;
                acceleration = {0.0f, 0.0f, 0.0f};
                for (int index{}; index < 3; ++index)
                {
                  auto drag = std::abs(velocity[index]) * (1 - (friction / max_velocity)) + friction;
                  if (velocity[index] > 0.0f)
                    velocity[index] = std::max(0.0f, velocity[index] - drag * poll_rate);
                  else if (velocity[index] < -0.0f)
                    velocity[index] = std::min(0.0f, velocity[index] + drag * poll_rate);
                  else
                    velocity[index] = 0.0f;
                }
                position += velocity * poll_rate;
                if (graphics.active.texture.transparency < 0.0) graphics.active.texture.transparency = 0.0;
                if (graphics.active.texture.transparency > 1.0) graphics.active.texture.transparency = 1.0;

                timers.call<void(const bool)>("texture_change", true);

                auto &animation{graphics.active.texture.animation};
                auto &group{graphics.active.texture.group};
                auto final{group.frames.size() - 1};
                if (group == texture::redhood.jump)
                  if (animation.frame == final && animation.elapsed >= group.frames[final].duration)
                  {
                    group = texture::redhood.idle;
                    animation = {0, 2.0, true};
                  }
                if (graphics.previous.texture.group == group && group == texture::redhood.idle)
                  if (animation.frame == 0 && graphics.previous.texture.animation.frame == final)
                  {
                    animation.speed = 1.0;
                    if (graphics.active.texture.color.r == 128)
                      graphics.active.texture.color.r = 32;
                    else
                      graphics.active.texture.color.r = 128;
                  }
                if (graphics.previous.texture.image == texture::shop.image &&
                    graphics.active.texture.image != texture::shop.image)
                  graphics.active.texture.color = {128, 128, 255, 255};
              });
  }

  environment::environment(const glm::ivec3 &translation_, const cse::image &image_, const cse::group &group_)
    : cse::object({translation_, {0, 0, 0}, {1, 1, 1}}, {vertex::main, fragment::main},
                  {image_, group_, {0, 0.0, false, 0.0}, {false, false}, {128, 128, 128, 255}, 1.0}, {0})
  {
  }
}
