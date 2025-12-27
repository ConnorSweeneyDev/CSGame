#include "camera.hpp"

#include <tuple>

#include "SDL3/SDL_scancode.h"
#include "cse/camera.hpp"
#include "glm/ext/vector_float3.hpp"

namespace csg
{
  custom_camera::custom_camera(const std::tuple<glm::vec3, glm::vec3, glm::vec3> &transform_)
    : camera(transform_, 45.0f)
  {
    hooks.add("input",
              [this](const bool *keys)
              {
                if (keys[SDL_SCANCODE_I]) state.translation.acceleration.y += 0.01f;
                if (keys[SDL_SCANCODE_K]) state.translation.acceleration.y -= 0.01f;
                if (keys[SDL_SCANCODE_L]) state.translation.acceleration.x += 0.01f;
                if (keys[SDL_SCANCODE_J]) state.translation.acceleration.x -= 0.01f;
                if (keys[SDL_SCANCODE_U]) state.translation.acceleration.z -= 0.01f;
                if (keys[SDL_SCANCODE_O]) state.translation.acceleration.z += 0.01f;
              });

    hooks.add("simulate",
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
}
