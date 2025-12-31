#include "cse/main.hpp"
#include "cse/exception.hpp"
#include "cse/game.hpp"
#include "cse/print.hpp"
#include "cse/system.hpp"

#include "game.hpp"

int cse::main(int argc, char *argv[])
{
  if (argc > 1 || !argv[0]) throw exception("Expected 1 argument, got {}", argc);
  game::create<csg::game>()->run();
  if constexpr (debug) print<COUT>("Exiting application...\n");
  return success;
}
