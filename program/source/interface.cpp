#include "interface.hpp"

#include "cse/collision.hpp"
#include "cse/interface.hpp"
#include "cse/print.hpp"
#include "cse/system.hpp"
#include "glm/ext/vector_double2.hpp"

#include "resource.hpp"

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

  void icon::on_simulate(const double) { state.active.timer.call<void()>("hide_text"); }

  void icon::on_hover(const cse::hitbox)
  {
    state.active.text = "Hi";
    state.active.timer.remove("hide_text");
  }

  void icon::on_unhover(const cse::hitbox)
  {
    state.active.text = "Bye";
    state.active.timer.set("hide_text", 0.5, [this]() { state.active.text.clear(); });
  }

  void icon::on_press(const cse::hitbox) { graphics.active.text.color = {1.0, 0.0, 0.0, 1.0}; }

  void icon::on_release(const cse::hitbox) { graphics.active.text.color = {1.0, 1.0, 1.0, 1.0}; }

  void icon::on_click(const cse::hitbox)
  {
    if (!cse::debug) return;
    cse::print<COUT>("{}: CLICKED\n", name.string());
  }
}
