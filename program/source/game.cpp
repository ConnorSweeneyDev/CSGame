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
    set_current_scene("main",
                      [](const std::shared_ptr<scene> scene)
                      {
                        scene->set_camera<camera>({{0.0f, 0.0f, 80.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}});
                        scene->set_object<player>("player", {{0, 0, 0}, {0, 0, 0}, {1, 1, 1}});
                        scene->set_object<environment>("floor", {{0, -61, 0}, {0, 0, 0}, {1, 1, 1}},
                                                       texture::floor.image, texture::floor.main);
                        scene->set_object<environment>("shop", {{80, 24, -1}, {0, 0, 0}, {1, 1, 1}},
                                                       texture::shop.image, texture::shop.main);
                        scene->set_object<environment>("background1", {{0, 80, -3}, {0, 0, 0}, {1, 1, 1}},
                                                       texture::background1.image, texture::background1.main);
                        scene->set_object<environment>("background2", {{0, 80, -6}, {0, 0, 0}, {1, 1, 1}},
                                                       texture::background2.image, texture::background2.main);
                        scene->set_object<environment>("background3", {{0, 80, -9}, {0, 0, 0}, {1, 1, 1}},
                                                       texture::background3.image, texture::background3.main);
                      });
  }
}
