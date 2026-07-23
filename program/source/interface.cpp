#include "interface.hpp"

#include <string>

#include "SDL3/SDL_mouse.h"
#include "cse/game.hpp"
#include "cse/interface.hpp"
#include "cse/resource.hpp"
#include "cse/scene.hpp"
#include "cse/window.hpp"
#include "glm/ext/vector_double2.hpp"

#include "resource.hpp"

namespace csg
{
  cursor::cursor()
    : cse::interface({.translation = {.value = {0.0, 0.0}, .interpolate = false},
                      .rotation = {.value = 0.0, .interpolate = false},
                      .scale = {.value = {1.0, 1.0}, .interpolate = false},
                      .interactable = false,
                      .texture = {.image = image::cursor,
                                  .animation = animation::cursor.main,
                                  .playback = {.frame = 0, .speed = {0.0}, .loop = false, .elapsed = 0.0},
                                  .flip = {.horizontal = false, .vertical = false},
                                  .color = {.tint = {.value = {0.5, 0.5, 0.5, 1.0}, .interpolate = true},
                                            .alpha = {.value = 1.0, .interpolate = true}}},
                      .text = {.content = "",
                               .font = {},
                               .animation = {},
                               .playback = {.frame = 0, .speed = {0.0}, .loop = false, .elapsed = 0.0},
                               .align = {.horizontal = {.preset = CENTER, .spacing = {0.0}},
                                         .vertical = {.preset = MIDDLE, .spacing = {0.0}},
                                         .offset = {.value = {0.0, 0.0}, .interpolate = true}},
                               .scale = {.value = {1.0, 1.0}, .interpolate = true},
                               .overflow = {.wrap = false, .clip = true},
                               .color = {.tint = {.value = {0.5, 0.5, 0.5, 1.0}, .interpolate = true},
                                         .alpha = {.value = 1.0, .interpolate = true}}},
                      .priority = {-1000, 1000}})
  {
  }

  void cursor::on_simulate(const double)
  {
    const auto &mouse{game->active.window->active.mouse};
    active.translation.value = mouse.position;
    active.texture.color.alpha.value = SDL_CursorVisible() ? 0.0 : 1.0;
  }

  text::text(const glm::dvec2 &translation_, const glm::dvec2 &scale_)
    : cse::interface({.translation = {.value = translation_, .interpolate = true},
                      .rotation = {.value = 0.0, .interpolate = true},
                      .scale = {.value = scale_, .interpolate = true},
                      .interactable = false,
                      .texture = {.image = image::empty,
                                  .animation = animation::empty.main,
                                  .playback = {.frame = 0, .speed = {0.0}, .loop = false, .elapsed = 0.0},
                                  .flip = {.horizontal = false, .vertical = false},
                                  .color = {.tint = {.value = {0.5, 0.5, 0.5, 1.0}, .interpolate = true},
                                            .alpha = {.value = 1.0, .interpolate = true}}},
                      .text = {.content = "",
                               .font = font::text,
                               .animation = animation::text.main,
                               .playback = {.frame = 0, .speed = {0.0}, .loop = false, .elapsed = 0.0},
                               .align = {.horizontal = {.preset = LEFT, .spacing = {-1.0}},
                                         .vertical = {.preset = MIDDLE, .spacing = {0.0}},
                                         .offset = {.value = {0.0, 0.0}, .interpolate = true}},
                               .scale = {.value = {1.0, 1.0}, .interpolate = true},
                               .overflow = {.wrap = false, .clip = true},
                               .color = {.tint = {.value = {0.5, 0.5, 0.5, 1.0}, .interpolate = true},
                                         .alpha = {.value = 1.0, .interpolate = true}}},
                      .priority = {-100, 100}})
  {
  }

  button::button(const glm::dvec2 &translation_)
    : cse::interface({.translation = {.value = translation_, .interpolate = true},
                      .rotation = {.value = 0.0, .interpolate = true},
                      .scale = {.value = {1.0, 1.0}, .interpolate = true},
                      .interactable = true,
                      .texture = {.image = image::box,
                                  .animation = animation::box.main,
                                  .playback = {.frame = 0, .speed = {0.0}, .loop = false, .elapsed = 0.0},
                                  .flip = {.horizontal = false, .vertical = false},
                                  .color = {.tint = {.value = {0.5, 0.5, 0.5, 1.0}, .interpolate = true},
                                            .alpha = {.value = 1.0, .interpolate = true}}},
                      .text = {.content = "",
                               .font = font::text,
                               .animation = animation::text.main,
                               .playback = {.frame = 0, .speed = {0.0}, .loop = false, .elapsed = 0.0},
                               .align = {.horizontal = {.preset = CENTER, .spacing = {-1.0}},
                                         .vertical = {.preset = MIDDLE, .spacing = {0.0}},
                                         .offset = {.value = {0.0, 0.0}, .interpolate = true}},
                               .scale = {.value = {1.0, 1.0}, .interpolate = true},
                               .overflow = {.wrap = false, .clip = true},
                               .color = {.tint = {.value = {0.5, 0.5, 0.5, 1.0}, .interpolate = true},
                                         .alpha = {.value = 1.0, .interpolate = true}}},
                      .priority = {0, 0}})
  {
  }

  void button::on_prepare()
  {
    active.mixer.set({{"once", sound::once}, {"twice", sound::twice}});
    if (name == "button1") active.priority = {1, 1};
  }

  void button::on_simulate(const double)
  {
    const auto &mouse{game->active.window->active.mouse};

    const auto hover = active.target.hovered == hitbox::box.main && previous.target.hovered != hitbox::box.main;
    const auto unhover = active.target.hovered != hitbox::box.main && previous.target.hovered == hitbox::box.main;
    if (hover)
    {
      active.text.content = "Hi";
      active.timer.remove("hide_text");
    }
    else if (unhover)
    {
      active.text.content = "Bye";
      active.timer.set("hide_text", [this]() { active.text.content.clear(); }).target = 0.5;
    }

    const auto hovered = active.target.hovered == hitbox::box.main;
    if (hovered)
    {
      active.translation.value.x += mouse.wheel.x;
      active.translation.value.y += mouse.wheel.y;
    }

    const auto right_press = active.target.pressed[SDL_BUTTON_RIGHT] == hitbox::box.main;
    const auto left_press = active.target.pressed[SDL_BUTTON_LEFT] == hitbox::box.main;
    if (right_press)
    {
      active.translation.value.x = mouse.position.x;
      active.translation.value.y = mouse.position.y;
      active.text.color.tint.value = {0.3, 0.3, 0.5, 1.0};
    }
    if (left_press) active.text.color.tint.value = {0.5, 0.3, 0.3, 1.0};
    if (!right_press && !left_press) active.text.color.tint.value = {0.5, 0.5, 0.5, 1.0};

    const auto left_click =
      active.target.clicked[SDL_BUTTON_LEFT] == hitbox::box.main && active.target.hovered == hitbox::box.main;
    if (left_click && scene)
    {
      auto &scene_mixer = (*scene)->active.mixer;
      if (name == "button1")
      {
        auto &song = scene_mixer.get<cse::music>("main");
        song.playing = !song.playing;
        auto &sfx = active.mixer.get<cse::sound>("once");
        sfx.position = 0;
        sfx.playing = true;
      }
      else if (name == "button2")
      {
        auto &song = scene_mixer.get<cse::music>("main");
        song.position = 0.0;
        song.speed.value = song.speed.value + 0.1;
      }
    }

    active.timer.call<void()>("hide_text");
  }
}
