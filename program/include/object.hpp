#pragma once

#include "SDL3/SDL_events.h"
#include "cse/object.hpp"
#include "cse/resource.hpp"
#include "glm/ext/vector_double3.hpp"

namespace csg
{
  class player final : public cse::object
  {
  public:
    player(const glm::dvec3 &translation_);

  protected:
    void on_event(const SDL_Event &event) final;
    void on_simulate(const double tick) final;

  private:
    static constexpr double max_velocity{200.0};
    static constexpr double friction{50.0};
    static constexpr double transparency_change{0.5};
  };

  class environment final : public cse::object
  {
  public:
    environment(const glm::dvec3 &translation_, const cse::image &image_, const cse::animation &animation_);

  protected:
    void on_prepare() final;
    void on_collide(const double tick) final;
  };
}
