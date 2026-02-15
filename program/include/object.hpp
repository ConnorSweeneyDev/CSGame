#pragma once

#include "SDL3/SDL_events.h"
#include "cse/object.hpp"
#include "cse/resource.hpp"
#include "glm/ext/vector_int3.hpp"

namespace csg
{
  class player final : public cse::object
  {
  public:
    player(const glm::ivec3 &translation_);

  protected:
    void on_event(const SDL_Event &event) override final;
    void on_input(const bool *keys) override final;
    void on_simulate(const double delta) override final;
    void on_collide(const double delta) override final;

  private:
    double max_velocity{200.0};
    double friction{50.0};
    double transparency_change{0.5};
  };

  class environment final : public cse::object
  {
  public:
    environment(const glm::ivec3 &translation_, const cse::image &image_, const cse::animation &animation_);
  };
}
