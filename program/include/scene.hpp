#pragma once

#include <memory>

#include "SDL3/SDL_events.h"
#include "cse/scene.hpp"

namespace csg
{
  class scene final : public cse::scene
  {
  public:
    scene();
    static void main(const std::shared_ptr<scene> scene);
    static void other(const std::shared_ptr<scene> scene);

  protected:
    void pre_event(const SDL_Event &event) override final;
    void pre_simulate(const double tick) override final;
  };
}
