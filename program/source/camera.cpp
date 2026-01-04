#include "camera.hpp"

#include <algorithm>

#include "SDL3/SDL_scancode.h"
#include "cse/camera.hpp"
#include "glm/ext/vector_float3.hpp"

namespace csg
{
  camera::camera(const glm::vec3 &translation_)
    : cse::camera({translation_, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}}, 45.0f)
  {
    hook.set("input",
             [this](const bool *keys)
             {
               auto &acceleration{state.active.translation.acceleration};
               auto difference{50.0f};
               if (keys[SDL_SCANCODE_I]) acceleration.y += difference;
               if (keys[SDL_SCANCODE_K]) acceleration.y -= difference;
               if (keys[SDL_SCANCODE_L]) acceleration.x += difference;
               if (keys[SDL_SCANCODE_J]) acceleration.x -= difference;
               if (keys[SDL_SCANCODE_U]) acceleration.z -= difference;
               if (keys[SDL_SCANCODE_O]) acceleration.z += difference;
             });

    hook.set("simulate",
             [this](const float poll_rate)
             {
               auto &velocity{state.active.translation.velocity};
               auto &acceleration{state.active.translation.acceleration};
               auto &value{state.active.translation.value};
               auto friction{10.0f};
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
