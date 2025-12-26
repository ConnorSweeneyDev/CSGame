#include "object.hpp"

#include <tuple>

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_scancode.h"
#include "cse/object.hpp"
#include "cse/resource.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_int3.hpp"

#include "resource.hpp"

namespace csg
{
  player::player(const std::tuple<glm::ivec3, glm::ivec3, glm::ivec3> &transform_)
    : object(transform_, {128, 128, 128, 0}, {csg::vertex::main, csg::fragment::main},
             {csg::texture::main::image, csg::texture::main::group::main})
  {
    hooks.add("event_main",
              [this](const SDL_Event &event)
              {
                if (event.type != SDL_EVENT_KEY_DOWN && event.type != SDL_EVENT_KEY_UP) return;
                switch (const auto &key{event.key}; key.scancode)
                {
                  case SDL_SCANCODE_0:
                    if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
                      graphics.texture.group = csg::texture::main::group::other;
                    else if (key.type == SDL_EVENT_KEY_UP)
                      graphics.texture.group = csg::texture::main::group::main;
                    break;
                  default: break;
                }
              });

    hooks.add("input_main",
              [this](const bool *keys)
              {
                if (keys[SDL_SCANCODE_E]) state.translation.acceleration.y += 0.01f;
                if (keys[SDL_SCANCODE_D]) state.translation.acceleration.y -= 0.01f;
                if (keys[SDL_SCANCODE_F]) state.translation.acceleration.x += 0.01f;
                if (keys[SDL_SCANCODE_S]) state.translation.acceleration.x -= 0.01f;
                if (keys[SDL_SCANCODE_W]) state.translation.acceleration.z += 0.01f;
                if (keys[SDL_SCANCODE_R]) state.translation.acceleration.z -= 0.01f;
              });

    hooks.add("simulate_main",
              [this]()
              {
                state.translation.velocity += state.translation.acceleration;
                state.translation.acceleration = glm::vec3{-0.002f};
                for (int index{}; index < 3; ++index)
                {
                  if (state.translation.velocity[index] < 0.0f)
                    state.translation.velocity[index] -= state.translation.acceleration[index];
                  if (state.translation.velocity[index] > 0.0f)
                    state.translation.velocity[index] += state.translation.acceleration[index];
                  if (state.translation.velocity[index] < 0.002f && state.translation.velocity[index] > -0.002f)
                    state.translation.velocity[index] = 0.0f;
                }
                state.translation.acceleration = glm::vec3{0.0f};
                state.translation.value += state.translation.velocity;
              });
  }

  environment::environment(const std::tuple<glm::ivec3, glm::ivec3, glm::ivec3> &transform_,
                           const cse::compiled_image &image_, const cse::compiled_frame_group &frame_group_)
    : object(transform_, {128, 128, 128, 0}, {csg::vertex::main, csg::fragment::main}, {image_, frame_group_})
  {
  }
}
