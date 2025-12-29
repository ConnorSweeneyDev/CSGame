#include "object.hpp"

#include <tuple>

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_scancode.h"
#include "cse/object.hpp"
#include "cse/resource.hpp"
#include "cse/scene.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_int3.hpp"

#include "resource.hpp"

namespace csg
{
  player::player(const std::tuple<glm::ivec3, glm::ivec3, glm::ivec3> &transform_)
    : object(transform_, {128, 128, 128, 255}, {csg::vertex::main, csg::fragment::main},
             {csg::texture::redhood.image, csg::texture::redhood.idle})
  {
    hooks.add("event",
              [this](const SDL_Event &event)
              {
                if (event.type != SDL_EVENT_KEY_DOWN && event.type != SDL_EVENT_KEY_UP) return;
                switch (const auto &key{event.key}; key.scancode)
                {
                  case SDL_SCANCODE_5:
                    if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
                      if (auto scene{parent.lock()}) scene->remove_object("player");
                    break;
                  case SDL_SCANCODE_G:
                    if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
                    {
                      auto &animation{graphics.texture.animation};
                      auto &group{graphics.texture.group};
                      if (group == csg::texture::redhood.idle)
                      {
                        group = csg::texture::redhood.jump;
                        animation.frame = 0;
                        animation.elapsed = 0.0;
                        animation.speed = 1.0;
                      }
                    }
                    break;
                  default: break;
                }
              });

    hooks.add("input",
              [this](const bool *keys)
              {
                if (keys[SDL_SCANCODE_E]) state.translation.acceleration.y += 0.01f;
                if (keys[SDL_SCANCODE_D]) state.translation.acceleration.y -= 0.01f;
                if (keys[SDL_SCANCODE_F]) state.translation.acceleration.x += 0.01f;
                if (keys[SDL_SCANCODE_S]) state.translation.acceleration.x -= 0.01f;
                if (keys[SDL_SCANCODE_W]) state.translation.acceleration.z += 0.01f;
                if (keys[SDL_SCANCODE_R]) state.translation.acceleration.z -= 0.01f;
              });

    hooks.add("simulate",
              [this]()
              {
                auto &velocity{state.translation.velocity};
                auto &acceleration{state.translation.acceleration};
                auto &value{state.translation.value};
                velocity += acceleration;
                acceleration = {-0.002f, -0.002f, -0.002f};
                for (int index{}; index < 3; ++index)
                {
                  if (velocity[index] < 0.0f) velocity[index] -= acceleration[index];
                  if (velocity[index] > 0.0f) velocity[index] += acceleration[index];
                  if (velocity[index] < 0.002f && velocity[index] > -0.002f) velocity[index] = 0.0f;
                }
                acceleration = {0.0f, 0.0f, 0.0f};
                value += velocity;

                auto &animation{graphics.texture.animation};
                auto &group{graphics.texture.group};
                auto &previous{graphics.texture.previous};
                auto last{group.frames.size() - 1};
                if (group == csg::texture::redhood.jump)
                {
                  if (animation.frame == last && animation.elapsed >= group.frames[last].duration)
                  {
                    graphics.texture.group = csg::texture::redhood.idle;
                    animation.frame = 0;
                    animation.elapsed = 0.0;
                    animation.speed = 2.0;
                  }
                }
                else if (animation.frame == last && animation.elapsed >= group.frames[last].duration)
                {
                  animation.frame = 0;
                  animation.elapsed = 0.0;
                  animation.speed = 1.0;
                }
                if (previous.group == group && group == csg::texture::redhood.idle)
                  if (previous.frame == last && animation.frame == 0)
                  {
                    if (graphics.color.r == 128)
                      graphics.color.r = 32;
                    else
                      graphics.color.r = 128;
                  }
              });
  }

  environment::environment(const std::tuple<glm::ivec3, glm::ivec3, glm::ivec3> &transform_, const cse::image &image_,
                           const cse::group &group_)
    : object(transform_, {128, 128, 128, 0}, {csg::vertex::main, csg::fragment::main}, {image_, group_})
  {
  }
}
