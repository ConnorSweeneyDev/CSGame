#include <memory>

#include "glm/ext/vector_double3.hpp"
#include "glm/ext/vector_int3.hpp"

#include "camera.hpp"
#include "game.hpp"
#include "object.hpp"
#include "resource.hpp"
#include "scene.hpp"
#include "window.hpp"

namespace csg
{
  void game::setup(const std::shared_ptr<game> game) { game->set<csg::window>().current("main", scene::main); }

  void scene::main(const std::shared_ptr<scene> scene)
  {
    scene->set<csg::camera>(glm::dvec3{0.0, 0.0, 80.0})
      .set<player>("player", glm::ivec3{0, 0, 0})
      .set<environment>("floor", glm::ivec3{0, -61, 0}, image::floor, animation::floor.main)
      .set<environment>("shop", glm::ivec3{80, 24, -1}, image::shop, animation::shop.main)
      .set<environment>("background1", glm::ivec3{0, 80, -3}, image::background1, animation::background1.main)
      .set<environment>("background2", glm::ivec3{0, 80, -6}, image::background2, animation::background2.main)
      .set<environment>("background3", glm::ivec3{0, 80, -9}, image::background3, animation::background3.main);
  }

  void scene::other(const std::shared_ptr<scene> scene)
  {
    scene->set<csg::camera>(glm::dvec3{0.0, 0.0, 80.0})
      .set<player>("player", glm::ivec3{0, 0, 0})
      .set<environment>("floor", glm::ivec3{0, -61, 0}, image::floor, animation::floor.main)
      .set<environment>("background3", glm::ivec3{0, 80, -9}, image::background3, animation::background3.main);
  }
}
