#include "cse/main.hpp"
#include "cse/game.hpp"

#include "game.hpp"

auto cse::main(const arguments &) -> application
{ return {game::create(csg::game::setup), {"ConnorSweeneyDev", "CSGame", "1.0.0"}}; }
