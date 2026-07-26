#include "light.hpp"

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_scancode.h"
#include "cse/light.hpp"
#include "cse/numeric.hpp"
#include "glm/ext/vector_double3.hpp"

sun::sun()
  : cse::light({.translation = {{0.0, 0.0, 0.0}},
                .rotation = {{0.0, 0.0}},
                .illumination = {.global = true,
                                 .brightness = {{0.15, 0.15, 0.2, 1.0}},
                                 .penetration = {1.0},
                                 .softness = {0.0},
                                 .range = {0.0},
                                 .angle = {0.0}},
                .shadow = {.cast = false, .darkness = {0.0}, .softness = {0.0}},
                .priority = 0}) {};

lamp::lamp(const glm::dvec3 &translation_)
  : cse::light({.translation = {translation_},
                .rotation = {{0.0, 0.0}},
                .illumination = {.global = false,
                                 .brightness = {{1.0, 0.7, 0.3, 2.0}},
                                 .penetration = {0.3},
                                 .softness = {0.0},
                                 .range = {80.0},
                                 .angle = {360.0}},
                .shadow = {.cast = true, .darkness = {1.0}, .softness = {0.3}},
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
  : cse::light({.translation = {translation_},
                .rotation = {{0.0, 90.0}},
                .illumination = {.global = false,
                                 .brightness = {{0.45, 1.05, 1.5, 1.0}},
                                 .penetration = {1.0},
                                 .softness = {1.0},
                                 .range = {90.0},
                                 .angle = {70.0}},
                .shadow = {.cast = true, .darkness = {1.0}, .softness = {0.0}},
                .priority = 0}) {};
