#pragma once

#include <memory>

#include "SDL3/SDL_events.h"

#include "cse/game.hpp"

namespace csg
{
  class game final : public cse::game
  {
  public:
    game();

    static void setup(const std::shared_ptr<game> &g);

  protected:
    void pre_prepare() final;
    void pre_event(const SDL_Event &event) final;
    void pre_simulate(const double tick) final;
    void post_clean() final;
  };
}
