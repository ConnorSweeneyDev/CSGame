#include <cstdlib>
#include <exception>
#include <memory>
#include <string>
#include <tuple>

#include "SDL3/SDL_main.h"
#include "cse/exception.hpp"
#include "cse/game.hpp"
#include "cse/print.hpp"
#include "cse/system.hpp"

#include "camera.hpp"
#include "object.hpp"
#include "scene.hpp"
#include "window.hpp"

int try_main(int argc, char *argv[])
{
  if (argc > 1 || !argv[0]) throw cse::utility::exception("Expected 1 argument, got {}", argc);

  auto game{std::make_unique<cse::core::game>()};
  game->set_window<csg::custom_window>("CSE Example", {1280, 720});

  if (auto scene{game->add_scene<csg::custom_scene>("scene").lock()})
  {
    scene->set_camera<csg::custom_camera>({{0.0f, 0.0f, 80.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}});
    scene->add_object<csg::custom_object>("object", {{0, 0, 0}, {0, 0, 0}, {1, 1, 1}});
  }
  game->set_current_scene("scene");

  game->run();
  game.reset();

  if constexpr (cse::system::debug) cse::utility::print<COUT>("Exiting application...\n");
  return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
  try
  {
    return try_main(argc, argv);
  }
  catch (const std::exception &error)
  {
    cse::utility::print<CERR>("{}\n", error.what());
    return EXIT_FAILURE;
  }
}
