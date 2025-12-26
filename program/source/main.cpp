#include <memory>
#include <string>
#include <tuple>

#include "cse/exception.hpp"
#include "cse/game.hpp"
#include "cse/main.hpp"
#include "cse/print.hpp"
#include "cse/system.hpp"

#include "camera.hpp"
#include "object.hpp"
#include "resource.hpp"
#include "scene.hpp"
#include "window.hpp"

int cse::main(int argc, char *argv[])
{
  if (argc > 1 || !argv[0]) throw cse::exception("Expected 1 argument, got {}", argc);

  auto game{cse::game::create()};
  game->set_window<csg::custom_window>("CSE Example", {1280, 720});
  game->set_current_scene(
    "scene",
    [](const std::shared_ptr<csg::custom_scene> scene)
    {
      scene->set_camera<csg::custom_camera>({{0.0f, 0.0f, 80.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}});
      scene->set_object<csg::player>("player", {{0, 0, 0}, {0, 0, 0}, {1, 1, 1}});
      scene->set_object<csg::environment>("floor", {{0, -61, 0}, {0, 0, 0}, {1, 1, 1}}, csg::texture::floor.image,
                                          csg::texture::floor.group.main);
      scene->set_object<csg::environment>("shop", {{80, 25, 0}, {0, 0, 0}, {1, 1, 1}}, csg::texture::shop.image,
                                          csg::texture::shop.group.main);
      scene->set_object<csg::environment>("background1", {{0, 80, -3}, {0, 0, 0}, {1, 1, 1}},
                                          csg::texture::background1.image, csg::texture::background1.group.main);
      scene->set_object<csg::environment>("background2", {{0, 80, -6}, {0, 0, 0}, {1, 1, 1}},
                                          csg::texture::background2.image, csg::texture::background2.group.main);
      scene->set_object<csg::environment>("background3", {{0, 80, -9}, {0, 0, 0}, {1, 1, 1}},
                                          csg::texture::background3.image, csg::texture::background3.group.main);
    });
  game->run();
  game.reset();

  if constexpr (cse::debug) cse::print<COUT>("Exiting application...\n");
  return cse::success;
}
