#include <string_view>
#include <vector>

#include "cse/game.hpp"
#include "cse/main.hpp"
#include "cse/system.hpp"

#include "game.hpp"

int cse::main(const std::vector<std::string_view> &)
{
  game::create(csg::game::setup)->run();
  return success;
}
