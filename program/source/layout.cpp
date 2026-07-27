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
  void game::setup(const std::shared_ptr<game> &g)
  {
    g->set<settings>("settings");
    g->set<window>();
    g->current("main", scene::main);
    g->set<cursor>("cursor");
    g->set<text>("tick", glm::dvec2{-133.0, 85.0}, glm::dvec2{50.0, 10.0});
    g->set<text>("frame", glm::dvec2{145.0, 85.0}, glm::dvec2{50.0, 10.0});
  }

  void scene::main(const std::shared_ptr<scene> &s)
  {
    s->set<button>("button1", glm::dvec2{-25.0, 79.0});
    s->set<button>("button2", glm::dvec2{25.0, 79.0});
    s->set<camera>(glm::dvec3{0.0, 0.0, 80.0});
    s->set<player>("player", glm::dvec3{0.0, -6.0, 0.0});
    s->set<environment>("floor", glm::dvec3{0.0, -61.0, 0.0}, image::floor, animation::floor.main);
    s->set<environment>("shop", glm::dvec3{80.0, 24.0, -1.0}, image::shop, animation::shop.main);
    s->set<environment>("background1", glm::dvec3{0.0, 80.0, -3.0}, image::background1, animation::background1.main);
    s->set<environment>("background2", glm::dvec3{0.0, 80.0, -6.0}, image::background2, animation::background2.main);
    s->set<environment>("background3", glm::dvec3{0.0, 80.0, -9.0}, image::background3, animation::background3.main);
    s->set<sun>("sun");
    s->set<lamp>("lamp1", glm::dvec3{32.0, 3.0, 0.0});
    s->set<lamp>("lamp2", glm::dvec3{130.0, 3.0, 0.0});
    s->set<spot>("spot", glm::dvec3{-80, 20.0, -9.0});
  }

  void scene::other(const std::shared_ptr<scene> &s)
  {
    s->set<camera>(glm::dvec3{0.0, 0.0, 80.0});
    s->set<player>("player", glm::dvec3{0.0, -6.0, 0.0});
    s->set<environment>("floor", glm::dvec3{0.0, -61.0, 0.0}, image::floor, animation::floor.main);
    s->set<environment>("background3", glm::dvec3{0.0, 80.0, -9.0}, image::background3, animation::background3.main);
    s->set<sun>("sun");
  }
}
