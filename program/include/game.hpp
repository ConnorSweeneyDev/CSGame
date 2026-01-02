#pragma once

#include <memory>

#include "cse/game.hpp"

namespace csg
{
  class game final : public cse::game
  {
  public:
    game();

    static void setup(const std::shared_ptr<game> game);
  };
}
