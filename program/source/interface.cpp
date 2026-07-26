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
    : cse::interface(
        {.translation = {{0.0, 0.0}},
         .rotation = {0.0},
         .scale = {{1.0, 1.0}},
         .interactable = false,
         .texture = {.source = {.image = image::cursor, .animation = animation::cursor.main},
                     .playback = {.frame = 0, .elapsed = 0.0, .playing = false, .speed = {0.0}, .loop = false},
                     .flip = {.horizontal = false, .vertical = false},
                     .color = {.tint = {{0.5, 0.5, 0.5, 1.0}}, .alpha = {1.0}}},
         .text = {.content = "",
                  .source = {.font = {}, .animation = {}},
                  .playback = {.frame = 0, .elapsed = 0.0, .playing = false, .speed = {0.0}, .loop = false},
                  .align = {.horizontal = {.preset = CENTER, .spacing = {0.0}},
                            .vertical = {.preset = MIDDLE, .spacing = {0.0}},
                            .offset = {{0.0, 0.0}}},
                  .scale = {{1.0, 1.0}},
                  .overflow = {.wrap = false, .clip = true},
                  .color = {.tint = {{0.5, 0.5, 0.5, 1.0}}, .alpha = {1.0}}},
         .priority = {-1000, 1000}}) {};

  void cursor::on_simulate(const double)
  {
    const auto &mouse{game->active.window->active.mouse};
    active.translation.value = mouse.position;
    active.texture.color.alpha.value = SDL_CursorVisible() ? 0.0 : 1.0;
  }

  text::text(const glm::dvec2 &translation_, const glm::dvec2 &scale_)
    : cse::interface(
        {.translation = {translation_},
         .rotation = {0.0},
         .scale = {scale_},
         .interactable = false,
         .texture = {.source = {.image = image::empty, .animation = animation::empty.main},
                     .playback = {.frame = 0, .elapsed = 0.0, .playing = false, .speed = {0.0}, .loop = false},
                     .flip = {.horizontal = false, .vertical = false},
                     .color = {.tint = {{0.5, 0.5, 0.5, 1.0}}, .alpha = {1.0}}},
         .text = {.content = "",
                  .source = {.font = font::text, .animation = animation::text.main},
                  .playback = {.frame = 0, .elapsed = 0.0, .playing = false, .speed = {0.0}, .loop = false},
                  .align = {.horizontal = {.preset = LEFT, .spacing = {-1.0}},
                            .vertical = {.preset = MIDDLE, .spacing = {0.0}},
                            .offset = {{0.0, 0.0}}},
                  .scale = {{1.0, 1.0}},
                  .overflow = {.wrap = false, .clip = true},
                  .color = {.tint = {{0.5, 0.5, 0.5, 1.0}}, .alpha = {1.0}}},
         .priority = {-100, 100}})
  {
  }

  button::button(const glm::dvec2 &translation_)
    : cse::interface(
        {.translation = {translation_},
         .rotation = {0.0},
         .scale = {{1.0, 1.0}},
         .interactable = true,
         .texture = {.source = {.image = image::box, .animation = animation::box.main},
                     .playback = {.frame = 0, .elapsed = 0.0, .playing = false, .speed = {0.0}, .loop = false},
                     .flip = {.horizontal = false, .vertical = false},
                     .color = {.tint = {{0.5, 0.5, 0.5, 1.0}}, .alpha = {1.0}}},
         .text = {.content = "",
                  .source = {.font = font::text, .animation = animation::text.main},
                  .playback = {.frame = 0, .elapsed = 0.0, .playing = false, .speed = {0.0}, .loop = false},
                  .align = {.horizontal = {.preset = CENTER, .spacing = {-1.0}},
                            .vertical = {.preset = MIDDLE, .spacing = {0.0}},
                            .offset = {{0.0, 0.0}}},
                  .scale = {{1.0, 1.0}},
                  .overflow = {.wrap = false, .clip = true},
                  .color = {.tint = {{0.5, 0.5, 0.5, 1.0}}, .alpha = {1.0}}},
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

    const auto hovered = active.target.hovered == hitbox::box.main;
    const auto hover = hovered && previous.target.hovered != hitbox::box.main;
    const auto unhover = !hovered && previous.target.hovered == hitbox::box.main;
    if (hovered)
    {
      active.translation.value.x += mouse.wheel.x;
      active.translation.value.y += mouse.wheel.y;
    }
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

    const auto right_press = active.target.pressed[SDL_BUTTON_RIGHT] == hitbox::box.main;
    const auto left_press = active.target.pressed[SDL_BUTTON_LEFT] == hitbox::box.main;
    const auto left_click = active.target.clicked[SDL_BUTTON_LEFT] == hitbox::box.main && hovered;
    if (right_press)
    {
      active.translation.value.x = mouse.position.x;
      active.translation.value.y = mouse.position.y;
      active.text.color.tint.value = {0.3, 0.3, 0.5, 1.0};
    }
    if (left_press) active.text.color.tint.value = {0.5, 0.3, 0.3, 1.0};
    if (!right_press && !left_press) active.text.color.tint.value = {0.5, 0.5, 0.5, 1.0};
    if (left_click && scene)
    {
      auto &scene_mixer = (*scene)->active.mixer;
      if (name == "button1")
      {
        auto &song = scene_mixer.get<cse::music>("main");
        song.playing = !song.playing;
        auto &sfx = active.mixer.get<cse::sound>("once");
        sfx.elapsed = {};
        sfx.playing = true;
      }
      else if (name == "button2")
      {
        auto &song = scene_mixer.get<cse::music>("main");
        song.elapsed = {};
        song.speed.value = song.speed.value + 0.1;
      }
    }

    active.timer.call<void()>("hide_text");
  }
}
