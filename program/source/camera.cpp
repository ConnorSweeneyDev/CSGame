#include "camera.hpp"

#include <algorithm>
#include <tuple>

#include "SDL3/SDL_scancode.h"
#include "cse/camera.hpp"
#include "glm/ext/vector_float3.hpp"

namespace csg
{
  camera::camera(const std::tuple<glm::vec3, glm::vec3, glm::vec3> &transform_) : cse::camera(transform_, 45.0f)
  {
    hook.set("input",
             [this](const bool *keys)
             {
               auto &acceleration{state.translation.acceleration};
               auto value{36.0f};
               if (keys[SDL_SCANCODE_I]) acceleration.y += value;
               if (keys[SDL_SCANCODE_K]) acceleration.y -= value;
               if (keys[SDL_SCANCODE_L]) acceleration.x += value;
               if (keys[SDL_SCANCODE_J]) acceleration.x -= value;
               if (keys[SDL_SCANCODE_U]) acceleration.z -= value;
               if (keys[SDL_SCANCODE_O]) acceleration.z += value;
             });

    hook.set("simulate",
             [this](const float poll_rate)
             {
               auto &velocity{state.translation.velocity};
               auto &acceleration{state.translation.acceleration};
               auto &value{state.translation.value};
               auto friction{7.2f};
               velocity += acceleration * poll_rate;
               acceleration = {0.0f, 0.0f, 0.0f};
               for (int index{}; index < 3; ++index)
                 if (velocity[index] > 0.0f)
                   velocity[index] = std::max(0.0f, velocity[index] - friction * poll_rate);
                 else if (velocity[index] < -0.0f)
                   velocity[index] = std::min(0.0f, velocity[index] + friction * poll_rate);
                 else
                   velocity[index] = 0.0f;
               value += velocity * poll_rate;
             });
  }
}
