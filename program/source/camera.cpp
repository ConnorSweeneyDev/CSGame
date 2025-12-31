#include "camera.hpp"

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
               if (keys[SDL_SCANCODE_I]) acceleration.y += 0.01f;
               if (keys[SDL_SCANCODE_K]) acceleration.y -= 0.01f;
               if (keys[SDL_SCANCODE_L]) acceleration.x += 0.01f;
               if (keys[SDL_SCANCODE_J]) acceleration.x -= 0.01f;
               if (keys[SDL_SCANCODE_U]) acceleration.z -= 0.01f;
               if (keys[SDL_SCANCODE_O]) acceleration.z += 0.01f;
             });

    hook.set("simulate",
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
             });
  }
}
