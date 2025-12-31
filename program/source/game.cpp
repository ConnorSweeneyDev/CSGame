#include "game.hpp"

#include <memory>

#include "cse/game.hpp"

#include "camera.hpp"
#include "object.hpp"
#include "resource.hpp"
#include "scene.hpp"
#include "window.hpp"

namespace csg
{
  game::game() : cse::game()
  {
    set_window<csg::window>("CSGame", {1280, 720});
    set_current_scene(
      "main",
      [](const std::shared_ptr<csg::scene> scene)
      {
        scene->set_camera<csg::camera>({{0.0f, 0.0f, 80.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}});
        scene->set_object<csg::player>("player", {{0, 0, 0}, {0, 0, 0}, {1, 1, 1}});
        scene->set_object<csg::environment>("floor", {{0, -61, 0}, {0, 0, 0}, {1, 1, 1}}, csg::texture::floor.image,
                                            csg::texture::floor.main);
        scene->set_object<csg::environment>("shop", {{80, 24, -1}, {0, 0, 0}, {1, 1, 1}}, csg::texture::shop.image,
                                            csg::texture::shop.main);
        scene->set_object<csg::environment>("background1", {{0, 80, -3}, {0, 0, 0}, {1, 1, 1}},
                                            csg::texture::background1.image, csg::texture::background1.main);
        scene->set_object<csg::environment>("background2", {{0, 80, -6}, {0, 0, 0}, {1, 1, 1}},
                                            csg::texture::background2.image, csg::texture::background2.main);
        scene->set_object<csg::environment>("background3", {{0, 80, -9}, {0, 0, 0}, {1, 1, 1}},
                                            csg::texture::background3.image, csg::texture::background3.main);
      });
  }

  std::shared_ptr<game> game::create() { return cse::game::create<game>(); }
}
