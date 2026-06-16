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
    : cse::camera(
        initial_state{
          .translation = translation_,
          .forward = {0.0, 0.0, -1.0},
          .up = {0.0, 1.0, 0.0},
        },
        initial_graphics{
          .fov = 45.0,
          .clip = {.near = 0.01, .far = 100.0},
        })
  {
  }

  void camera::on_simulate(const double tick)
  {
    const auto &keyboard{scene->game->state.active.window->state.active.keyboard};

    auto &position{state.active.translation.value};
    auto &velocity{state.active.translation.rate};
    auto &acceleration{state.active.translation.curve};
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

    auto &fov_value{graphics.active.fov.value};
    auto &fov_rate{graphics.active.fov.rate};
    if (keyboard[SDL_SCANCODE_H]) fov_rate -= fov_change;
    if (keyboard[SDL_SCANCODE_SEMICOLON]) fov_rate += fov_change;
    fov_value += fov_rate * tick;
    fov_rate = 0.0;
    if (graphics.active.fov.value < 30.0) graphics.active.fov = 30.0;
    if (graphics.active.fov.value > 60.0) graphics.active.fov = 60.0;
  }
}
