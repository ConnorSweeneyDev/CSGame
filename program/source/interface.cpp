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
        initial_state{
          .translation = {0.0, 0.0},
          .rotation = 0.0,
          .scale = {1.0, 1.0},
          .interactable = false,
          .priority = -1000,
        },
        initial_graphics{
          .shader = {.vertex = vertex::main, .fragment = fragment::main},
          .texture = {.image = image::cursor,
                      .animation = animation::cursor.main,
                      .playback = {.frame = 0, .speed = 0.0, .loop = false, .elapsed = 0.0},
                      .flip = {.horizontal = false, .vertical = false},
                      .color = {0.5, 0.5, 0.5, 1.0},
                      .transparency = 1.0},
          .text = {.content = "",
                   .font = {},
                   .size = 0,
                   .style = {.bold = false, .italic = false, .underline = false, .strikethrough = false},
                   .color = {0.0, 0.0, 0.0, 0.0},
                   .align = {.horizontal = cse::align::CENTER, .vertical = cse::align::MIDDLE, .offset = {0.0, 0.0}},
                   .spacing = 0.0,
                   .wrap = false,
                   .overflow = false},
          .priority = 1000,
        })
  {
  }

  void cursor::on_simulate(const double)
  {
    const auto &mouse{game->state.active.window->state.active.mouse};
    state.active.translation = mouse.position;
  }

  text::text(const glm::dvec2 &translation_, const glm::dvec2 &scale_)
    : cse::interface(
        initial_state{
          .translation = translation_,
          .rotation = 0.0,
          .scale = scale_,
          .interactable = false,
          .priority = -100,
        },
        initial_graphics{
          .shader = {.vertex = vertex::main, .fragment = fragment::main},
          .texture = {.image = image::empty,
                      .animation = animation::empty.main,
                      .playback = {.frame = 0, .speed = 0.0, .loop = false, .elapsed = 0.0},
                      .flip = {.horizontal = false, .vertical = false},
                      .color = {0.5, 0.5, 0.5, 1.0},
                      .transparency = 1.0},
          .text = {.content = "",
                   .font = font::main,
                   .size = 12,
                   .style = {.bold = false, .italic = false, .underline = false, .strikethrough = false},
                   .color = {1.0, 1.0, 1.0, 1.0},
                   .align = {.horizontal = cse::align::LEFT, .vertical = cse::align::MIDDLE, .offset = {0.0, 0.0}},
                   .spacing = 0.0,
                   .wrap = false,
                   .overflow = false},
          .priority = 100,
        })
  {
  }

  button::button(const glm::dvec2 &translation_)
    : cse::interface(
        initial_state{
          .translation = translation_,
          .rotation = 0.0,
          .scale = {1.0, 1.0},
          .interactable = true,
          .priority = 0,
        },
        initial_graphics{
          .shader = {.vertex = vertex::main, .fragment = fragment::main},
          .texture = {.image = image::box,
                      .animation = animation::box.main,
                      .playback = {.frame = 0, .speed = 0.0, .loop = false, .elapsed = 0.0},
                      .flip = {.horizontal = false, .vertical = false},
                      .color = {0.5, 0.5, 0.5, 1.0},
                      .transparency = 1.0},
          .text = {.content = "",
                   .font = font::main,
                   .size = 20,
                   .style = {.bold = true, .italic = false, .underline = false, .strikethrough = false},
                   .color = {1.0, 1.0, 1.0, 1.0},
                   .align = {.horizontal = cse::align::CENTER, .vertical = cse::align::MIDDLE, .offset = {0.0, 0.0}},
                   .spacing = 0.0,
                   .wrap = false,
                   .overflow = false},
          .priority = 0,
        })
  {
  }

  void button::on_prepare()
  {
    state.active.mixer.load({{"sample1", sound::sample1}, {"sample2", sound::sample2}, {"sample3", sound::sample3}});
    if (name == "button1")
    {
      state.active.priority = 1;
      graphics.active.priority = 1;
    }
  }

  void button::on_simulate(const double)
  {
    const auto &mouse{game->state.active.window->state.active.mouse};

    const auto hover =
      state.active.target.hovered == hitbox::box.main && state.previous.target.hovered != hitbox::box.main;
    const auto unhover =
      state.active.target.hovered != hitbox::box.main && state.previous.target.hovered == hitbox::box.main;
    if (hover)
    {
      graphics.active.text.content = "Hi";
      state.active.timer.remove("hide_text");
    }
    else if (unhover)
    {
      graphics.active.text.content = "Bye";
      state.active.timer.set("hide_text", 0.5, [this]() { graphics.active.text.content.clear(); });
    }

    const auto hovered = state.active.target.hovered == hitbox::box.main;
    if (hovered)
    {
      state.active.translation.value.x += mouse.wheel.x;
      state.active.translation.value.y -= mouse.wheel.y;
    }

    const auto right_press = state.active.target.pressed[SDL_BUTTON_RIGHT] == hitbox::box.main;
    const auto left_press = state.active.target.pressed[SDL_BUTTON_LEFT] == hitbox::box.main;
    if (right_press)
    {
      state.active.translation.value.x = mouse.position.x;
      state.active.translation.value.y = mouse.position.y;
      graphics.active.text.color = {0.0, 0.0, 1.0, 1.0};
    }
    if (left_press) graphics.active.text.color = {1.0, 0.0, 0.0, 1.0};
    if (!right_press && !left_press) graphics.active.text.color = {1.0, 1.0, 1.0, 1.0};

    const auto left_click = state.active.target.clicked[SDL_BUTTON_LEFT] == hitbox::box.main &&
                            state.active.target.hovered == hitbox::box.main;
    if (left_click && scene)
    {
      auto &scene_mixer = (*scene)->state.active.mixer;
      if (name == "button1")
      {
        auto &song = scene_mixer.get<cse::music>("main");
        song.playing = !song.playing;
        auto &sfx = state.active.mixer.get<cse::sound>("sample3");
        sfx.position = 0;
        sfx.playing = true;
      }
      else if (name == "button2")
      {
        auto &song = scene_mixer.get<cse::music>("main");
        song.position = 0.0;
        song.speed = song.speed.value + 0.1;
      }
    }

    state.active.timer.call<void()>("hide_text");
  }
}
