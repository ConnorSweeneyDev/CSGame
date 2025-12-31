#pragma once

#include "cse/game.hpp"
#include <memory>

namespace csg
{
  class game : public cse::game
  {
  public:
    game();

    static std::shared_ptr<game> create();
  };
}
