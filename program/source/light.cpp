#include "light.hpp"

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_scancode.h"
#include "cse/light.hpp"
#include "cse/numeric.hpp"
#include "glm/ext/vector_double3.hpp"

sun::sun()
  : cse::light({.translation = {.value = {0.0, 0.0, 0.0}, .interpolate = true},
                .rotation = {.value = {0.0, 0.0}, .interpolate = true},
                .illumination = {.global = true,
                                 .brightness = {.value = {0.15, 0.15, 0.2, 1.0}, .interpolate = true},
                                 .penetration = {.value = 1.0, .interpolate = true},
                                 .softness = {.value = 0.0, .interpolate = true},
                                 .range = {.value = 0.0, .interpolate = true},
                                 .angle = {.value = 0.0, .interpolate = true}},
                .shadow = {.cast = false,
                           .darkness = {.value = 0.0, .interpolate = true},
                           .softness = {.value = 0.0, .interpolate = true}},
                .priority = 0}) {};

lamp::lamp(const glm::dvec3 &translation_)
  : cse::light({.translation = {.value = translation_, .interpolate = true},
                .rotation = {.value = {0.0, 0.0}, .interpolate = true},
                .illumination = {.global = false,
                                 .brightness = {.value = {1.0, 0.7, 0.3, 2.0}, .interpolate = true},
                                 .penetration = {.value = 0.3, .interpolate = true},
                                 .softness = {.value = 0.0, .interpolate = true},
                                 .range = {.value = 80.0, .interpolate = true},
                                 .angle = {.value = 360.0, .interpolate = true}},
                .shadow = {.cast = true,
                           .darkness = {.value = 1.0, .interpolate = true},
                           .softness = {.value = 0.3, .interpolate = true}},
                .priority = 0}) {};

void lamp::on_event(const SDL_Event &event)
{
  if (event.type != SDL_EVENT_KEY_DOWN && event.type != SDL_EVENT_KEY_UP) return;
  switch (const auto &key{event.key}; key.scancode)
  {
    case SDL_SCANCODE_Y:
      if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
        active.illumination.angle.value = equal(active.illumination.angle.value, 170.0) ? 360.0 : 170.0;
      break;
    default: break;
  }
}

spot::spot(const glm::dvec3 &translation_)
  : cse::light({.translation = {.value = translation_, .interpolate = true},
                .rotation = {.value = {0.0, 90.0}, .interpolate = true},
                .illumination = {.global = false,
                                 .brightness = {.value = {0.45, 1.05, 1.5, 1.0}, .interpolate = true},
                                 .penetration = {.value = 1.0, .interpolate = true},
                                 .softness = {.value = 1.0, .interpolate = true},
                                 .range = {.value = 90.0, .interpolate = true},
                                 .angle = {.value = 70.0, .interpolate = true}},
                .shadow = {.cast = true,
                           .darkness = {.value = 1.0, .interpolate = true},
                           .softness = {.value = 0.0, .interpolate = true}},
                .priority = 0}) {};
