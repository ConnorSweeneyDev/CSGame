#include <memory>
#include <string_view>
#include <vector>

#include "cse/game.hpp"
#include "cse/main.hpp"

#include "game.hpp"

std::shared_ptr<cse::game> cse::main(const std::vector<std::string_view> &) { return game::create(csg::game::setup); }
