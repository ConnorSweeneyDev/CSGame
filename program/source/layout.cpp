#include <memory>

#include "glm/ext/vector_double2.hpp"
#include "glm/ext/vector_double3.hpp"

#include "camera.hpp"
#include "game.hpp"
#include "interface.hpp"
#include "light.hpp"
#include "object.hpp"
#include "resource.hpp"
#include "scene.hpp"
#include "state.hpp"
#include "window.hpp"

namespace csg
{
  void game::setup(const std::shared_ptr<game> &game)
  {
    game->set<settings>("settings")
      .set<window>()
      .current("main", scene::main)
      .set<cursor>("cursor")
      .set<text>("tick", glm::dvec2{-63.0, -46.0}, glm::dvec2{50, 10})
      .set<text>("frame", glm::dvec2{75.0, -46.0}, glm::dvec2{50, 10});
  }

  void scene::main(const std::shared_ptr<scene> &scene)
  {
    scene->set<button>("button1", glm::dvec2{-22.0, -40.0})
      .set<button>("button2", glm::dvec2{22.0, -40.0})
      .set<camera>(glm::dvec3{0.0, 0.0, 80.0})
      .set<player>("player", glm::dvec3{0.0, 0.0, 0.0})
      .set<environment>("floor", glm::dvec3{0.0, -61.0, 0.0}, image::floor, animation::floor.main)
      .set<environment>("shop", glm::dvec3{80.0, 24.0, -1.0}, image::shop, animation::shop.main)
      .set<environment>("background1", glm::dvec3{0.0, 80.0, -3.0}, image::background1, animation::background1.main)
      .set<environment>("background2", glm::dvec3{0.0, 80.0, -6.0}, image::background2, animation::background2.main)
      .set<environment>("background3", glm::dvec3{0.0, 80.0, -9.0}, image::background3, animation::background3.main)
      .set<sun>("sun")
      .set<lamp>("lamp1", glm::dvec3{32.0, 3.0, 0.0})
      .set<lamp>("lamp2", glm::dvec3{130.0, 3.0, 0.0})
      .set<spot>("spot", glm::dvec3{-80, 20.0, -9.0});
  }

  void scene::other(const std::shared_ptr<scene> &scene)
  {
    scene->set<camera>(glm::dvec3{0.0, 0.0, 80.0})
      .set<player>("player", glm::dvec3{0.0, 0.0, 0.0})
      .set<environment>("floor", glm::dvec3{0.0, -61.0, 0.0}, image::floor, animation::floor.main)
      .set<environment>("background3", glm::dvec3{0.0, 80.0, -9.0}, image::background3, animation::background3.main)
      .set<sun>("sun");
  }
}
