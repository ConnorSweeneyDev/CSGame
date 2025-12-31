#include "object.hpp"

#include <tuple>

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_scancode.h"
#include "cse/object.hpp"
#include "cse/resource.hpp"
#include "cse/scene.hpp"
#include "cse/utility.hpp"
#include "glm/ext/vector_int3.hpp"

#include "resource.hpp"

namespace csg
{
  player::player(const std::tuple<glm::ivec3, glm::ivec3, glm::ivec3> &transform_)
    : cse::object(transform_, {128, 128, 128, 255}, {vertex::main, fragment::main},
                  {texture::redhood.image, texture::redhood.idle, 0, 1.0, true})
  {
    hook.set("event",
             [this](const SDL_Event &event)
             {
               if (event.type != SDL_EVENT_KEY_DOWN && event.type != SDL_EVENT_KEY_UP) return;
               switch (const auto &key{event.key}; key.scancode)
               {
                 case SDL_SCANCODE_3:
                   if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
                   {
                     if (graphics.texture->image == csg::texture::redhood.image)
                       graphics.texture = {texture::shop.image, texture::shop.main};
                     else
                       graphics.texture = {texture::redhood.image, texture::redhood.idle, {0, 1.0, true}};
                   }
                   break;
                 case SDL_SCANCODE_4:
                   if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
                   {
                     if (auto scene{throw_lock(parent)}; scene->objects.contains("temp"))
                       scene->remove_object("temp");
                     else
                       scene->set_object<environment>("temp", {{-80, 24, -1}, {0, 0, 0}, {1, 1, 1}},
                                                      texture::shop.image, texture::shop.main);
                   }
                   break;
                 case SDL_SCANCODE_5:
                   if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN) throw_lock(parent)->remove_object("player");
                   break;
                 case SDL_SCANCODE_0:
                   if (!key.repeat && key.type == SDL_EVENT_KEY_DOWN)
                   {
                     auto &animation{graphics.texture->animation};
                     auto &group{graphics.texture->group};
                     if (group == texture::redhood.idle)
                     {
                       group = texture::redhood.jump;
                       animation = {0, 1.0, false};
                     }
                   }
                   break;
                 default: break;
               }
             });

    hook.set("input",
             [this](const bool *keys)
             {
               auto &acceleration{state.translation.acceleration};
               if (keys[SDL_SCANCODE_E]) acceleration.y += 0.01f;
               if (keys[SDL_SCANCODE_D]) acceleration.y -= 0.01f;
               if (keys[SDL_SCANCODE_F]) acceleration.x += 0.01f;
               if (keys[SDL_SCANCODE_S]) acceleration.x -= 0.01f;
               if (keys[SDL_SCANCODE_W]) acceleration.z += 0.01f;
               if (keys[SDL_SCANCODE_R]) acceleration.z -= 0.01f;
             });

    hook.set("simulate",
             [this]()
             {
               auto &velocity{state.translation.velocity};
               auto &acceleration{state.translation.acceleration};
               auto &value{state.translation.value};
               velocity += acceleration;
               acceleration = {-0.002f, -0.002f, -0.002f};
               for (int index{}; index < 3; ++index)
               {
                 if (velocity[index] < 0.0f) velocity[index] -= acceleration[index];
                 if (velocity[index] > 0.0f) velocity[index] += acceleration[index];
                 if (velocity[index] < 0.002f && velocity[index] > -0.002f) velocity[index] = 0.0f;
               }
               acceleration = {0.0f, 0.0f, 0.0f};
               value += velocity;

               auto &animation{graphics.texture->animation};
               auto &group{graphics.texture->group};
               auto &previous{graphics.previous};
               auto final{group.frames.size() - 1};
               if (group == texture::redhood.jump)
               {
                 if (animation.frame == final && animation.elapsed >= group.frames[final].duration)
                 {
                   group = texture::redhood.idle;
                   animation = {0, 2.0, true};
                 }
               }
               if (previous.texture.group == group && group == texture::redhood.idle)
                 if (animation.frame == 0 && previous.texture.animation.frame == final)
                 {
                   animation.speed = 1.0;
                   if (graphics.color.r == 128)
                     graphics.color.r = 32;
                   else
                     graphics.color.r = 128;
                 }
               if (previous.texture.image == texture::shop.image && graphics.texture->image != texture::shop.image)
                 graphics.color = {128, 128, 255, 255};
             });
  }

  environment::environment(const std::tuple<glm::ivec3, glm::ivec3, glm::ivec3> &transform_, const cse::image &image_,
                           const cse::group &group_)
    : cse::object(transform_, {128, 128, 128, 0}, {vertex::main, fragment::main}, {image_, group_, 0, 0.0, false})
  {
  }
}
