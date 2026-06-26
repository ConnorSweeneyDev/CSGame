#include "camera.hpp"

#include <algorithm>
#include <cstdlib>

#include "SDL3/SDL_scancode.h"
#include "cse/camera.hpp"
#include "cse/game.hpp"
#include "cse/scene.hpp"
#include "cse/window.hpp"
#include "glm/ext/vector_double3.hpp"

namespace csg
{
  camera::camera(const glm::dvec3 &translation_)
    : cse::camera({.translation = {.value = translation_, .interpolate = true},
                   .forward = {.value = {0.0, 0.0, -1.0}, .interpolate = true},
                   .up = {.value = {0.0, 1.0, 0.0}, .interpolate = true},
                   .fov = {.value = 45.0, .interpolate = true},
                   .clip = {.near = 0.01, .far = 100.0}})
  {
  }

  void camera::on_simulate(const double tick)
  {
    const auto &keyboard{scene->game->active.window->active.keyboard};

    auto &position{active.translation.value};
    auto &velocity{active.translation.rate};
    auto &acceleration{active.translation.curve};
    if (keyboard[SDL_SCANCODE_I]) acceleration.y += max_velocity;
    if (keyboard[SDL_SCANCODE_K]) acceleration.y -= max_velocity;
    if (keyboard[SDL_SCANCODE_L]) acceleration.x += max_velocity;
    if (keyboard[SDL_SCANCODE_J]) acceleration.x -= max_velocity;
    if (keyboard[SDL_SCANCODE_U]) acceleration.z -= max_velocity;
    if (keyboard[SDL_SCANCODE_O]) acceleration.z += max_velocity;
    velocity += acceleration * tick;
    acceleration = {0.0, 0.0, 0.0};
    for (int index{}; index < 3; ++index)
    {
      auto drag = std::abs(velocity[index]) * (1.0 - (friction / max_velocity)) + friction;
      if (velocity[index] > 0.0)
        velocity[index] = std::max(0.0, velocity[index] - drag * tick);
      else if (velocity[index] < -0.0)
        velocity[index] = std::min(0.0, velocity[index] + drag * tick);
      else
        velocity[index] = 0.0;
    }
    position += velocity * tick;

    auto &fov_value{active.fov.value};
    auto &fov_rate{active.fov.rate};
    if (keyboard[SDL_SCANCODE_H]) fov_rate -= fov_change;
    if (keyboard[SDL_SCANCODE_SEMICOLON]) fov_rate += fov_change;
    fov_value += fov_rate * tick;
    fov_rate = 0.0;
    if (active.fov.value < 30.0) active.fov.value = 30.0;
    if (active.fov.value > 60.0) active.fov.value = 60.0;
  }
}
