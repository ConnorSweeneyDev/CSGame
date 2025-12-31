#include "cse/main.hpp"
#include "cse/exception.hpp"
#include "cse/print.hpp"
#include "cse/system.hpp"

#include "game.hpp"

int cse::main(int argc, char *argv[])
{
  if (argc > 1 || !argv[0]) throw exception("Expected 1 argument, got {}", argc);
  csg::game::create()->run();
  if constexpr (debug) print<COUT>("Exiting application...\n");
  return success;
}
