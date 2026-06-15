#include "interface.hpp"

#include "SDL3/SDL_mouse.h"
#include "SDL3/SDL_stdinc.h"
#include "cse/interface.hpp"
#include "cse/resource.hpp"
#include "cse/scene.hpp"
#include "glm/ext/vector_double2.hpp"

#include "resource.hpp"
#include <cse/game.hpp>
#include <cse/window.hpp>

namespace csg
{
  icon::icon(const glm::dvec2 &translation_)
    : cse::interface(
        initial_state{
          .translation = translation_,
          .rotation = 0.0,
          .scale = {1.0, 1.0},
          .text = "",
          .priority = 0,
        },
        initial_graphics{
          .shader = {.vertex = vertex::main, .fragment = fragment::main},
          .texture = {.image = image::box, .animation = animation::box.main},
          .render = {.playback = {.frame = 0, .speed = 0.0, .loop = false, .elapsed = 0.0},
                     .flip = {.horizontal = false, .vertical = false},
                     .color = {0.5, 0.5, 0.5, 1.0},
                     .transparency = 1.0},
          .text = {.font = font::main, .size = 20, .color = {1.0, 1.0, 1.0, 1.0}},
          .priority = 0,
        })
  {
  }

  void icon::on_prepare()
  {
    state.active.mixer.load({{"sample1", sound::sample1}, {"sample2", sound::sample2}, {"sample3", sound::sample3}});
    if (name == "icon1")
    {
      state.active.priority = 1;
      graphics.active.priority = 1;
    }
  }

  void icon::on_simulate(const double)
  {
    state.active.timer.call<void()>("hide_text");
    const auto &mouse{scene ? scene->game->state.active.window->state.active.mouse
                            : game->state.active.window->state.active.mouse};
    if (is_dragging)
    {
      state.active.translation.value.x = mouse.position.x;
      state.active.translation.value.y = mouse.position.y;
      graphics.active.text.color = {0.0, 0.0, 1.0, 1.0};
    }
    if (is_red) graphics.active.text.color = {1.0, 0.0, 0.0, 1.0};
  }

  void icon::on_hover()
  {
    state.active.text = "Hi";
    state.active.timer.remove("hide_text");
  }

  void icon::on_unhover()
  {
    state.active.text = "Bye";
    state.active.timer.set("hide_text", 0.5, [this]() { state.active.text.clear(); });
  }

  void icon::on_press(const Uint8 button)
  {
    if (state.active.target.interacted[button] != hitbox::box.main) return;
    if (button == SDL_BUTTON_LEFT)
      is_red = true;
    else if (button == SDL_BUTTON_RIGHT)
      is_dragging = true;
  }

  void icon::on_release(const Uint8 button)
  {
    if (button == SDL_BUTTON_LEFT) is_red = false;
    if (button == SDL_BUTTON_RIGHT) is_dragging = false;
    graphics.active.text.color = {1.0, 1.0, 1.0, 1.0};
  }

  void icon::on_click(const Uint8 button)
  {
    if (button != SDL_BUTTON_LEFT) return;
    auto &scene_mixer = scene ? scene->state.active.mixer : game->state.active.scene->state.active.mixer;
    if (name == "icon1")
    {
      auto &song = scene_mixer.get<cse::music>("main");
      song.playing = !song.playing;
      auto &sfx = state.active.mixer.get<cse::sound>("sample3");
      sfx.position = 0;
      sfx.playing = true;
    }
    else if (name == "icon2")
    {
      auto &song = scene_mixer.get<cse::music>("main");
      song.position = 0.0;
      song.speed = song.speed.value + 0.1;
    }
  }

  void icon::on_scroll(const glm::dvec2 &delta)
  {
    state.active.translation.value.x += delta.x;
    state.active.translation.value.y -= delta.y;
  }
}
