#pragma once

#include <utility>

#include "cse/game.hpp"

namespace csg
{
  class game : public cse::game
  {
  public:
    game(const std::pair<double, double> &rates_);
  };
}
