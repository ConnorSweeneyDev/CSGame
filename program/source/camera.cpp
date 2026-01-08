#include "camera.hpp"

#include <algorithm>
#include <cstdlib>

#include "SDL3/SDL_scancode.h"
#include "cse/camera.hpp"
#include "glm/ext/vector_float3.hpp"

namespace csg
{
  camera::camera(const glm::vec3 &translation_)
    : cse::camera({translation_, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}}, 45.0)
  {
    hook.set(hooks::INPUT(),
             [this](const bool *keys)
             {
               auto &acceleration{state.active.translation.acceleration};
               if (keys[SDL_SCANCODE_I]) acceleration.y += max_velocity;
               if (keys[SDL_SCANCODE_K]) acceleration.y -= max_velocity;
               if (keys[SDL_SCANCODE_L]) acceleration.x += max_velocity;
               if (keys[SDL_SCANCODE_J]) acceleration.x -= max_velocity;
               if (keys[SDL_SCANCODE_U]) acceleration.z -= max_velocity;
               if (keys[SDL_SCANCODE_O]) acceleration.z += max_velocity;
               if (keys[SDL_SCANCODE_H]) graphics.active.fov -= 0.05;
               if (keys[SDL_SCANCODE_SEMICOLON]) graphics.active.fov += 0.05;
             });

    hook.set(hooks::SIMULATE(),
             [this](const float poll_rate)
             {
               auto &velocity{state.active.translation.velocity};
               auto &acceleration{state.active.translation.acceleration};
               auto &value{state.active.translation.value};
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
               value += velocity * poll_rate;
               if (graphics.active.fov < 30.0) graphics.active.fov = 30.0;
               if (graphics.active.fov > 60.0) graphics.active.fov = 60.0;
             });
  }
}
