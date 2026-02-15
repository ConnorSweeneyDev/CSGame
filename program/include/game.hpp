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
    static void setup(const std::shared_ptr<game> game);

  protected:
    void pre_event(const SDL_Event &event) override final;
    void pre_simulate(const double delta) override final;
  };
}
