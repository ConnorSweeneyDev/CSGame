#include "cse/main.hpp"
#include "cse/exception.hpp"
#include "cse/game.hpp"
#include "cse/system.hpp"

#include "game.hpp"

int cse::main(int argc, char *argv[])
{
  if (argc > 1 || !argv[0]) throw exception("Expected 1 argument, got {}", argc);
  game::create(csg::game::setup)->run();
  return success;
}
