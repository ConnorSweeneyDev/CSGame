#pragma once

#include "cse/locale.hpp"

namespace csg
{
  LANGUAGES(en, es, fr);

  TRANSLATE(player,          //
            (en, "Player"),  //
            (es, "Jugador"), //
            (fr, "Joueur"));
  TRANSLATE(greeting,     //
            (en, "Hi"),   //
            (es, "Hola"), //
            (fr, "Bonjour"));
  TRANSLATE(farewell,      //
            (en, "Bye"),   //
            (es, "Adios"), //
            (fr, "Au revoir"));
}
