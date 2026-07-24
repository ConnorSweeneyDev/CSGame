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
                   .clip = {.near = 10.0, .far = 400.0}}) {};

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
      auto &component{velocity[index]};
      const auto drag = (std::abs(component) * (1.0 - (friction / max_velocity))) + friction;
      if (component > 0.0)
        component = std::max(0.0, component - (drag * tick));
      else if (component < -0.0)
        component = std::min(0.0, component + (drag * tick));
      else
        component = 0.0;
    }
    position += velocity * tick;

    auto &forward_value{active.forward.value.x};
    auto &forward_rate{active.forward.rate.x};
    if (keyboard[SDL_SCANCODE_APOSTROPHE]) forward_rate -= forward_change;
    if (keyboard[SDL_SCANCODE_SEMICOLON]) forward_rate += forward_change;
    forward_value += forward_rate * tick;
    forward_rate = 0.0;
    active.fov.value = std::min(std::max(active.fov.value, 30.0), 60.0);
  }
}
