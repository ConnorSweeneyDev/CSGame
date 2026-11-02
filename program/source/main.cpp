#include "cse/main.hpp"
#include "cse/game.hpp"

#include "game.hpp"

auto cse::main(const arglist &) -> application
{ return {game::make(csg::game::setup), {"ConnorSweeneyDev", "CSGame", "1.0.0"}}; }
