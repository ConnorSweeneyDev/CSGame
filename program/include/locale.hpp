#pragma once

#include "cse/locale.hpp"

namespace csg
{
  LANGUAGES(EN, ES, FR);

  TRANSLATE(player,          //
            (EN, "Player"),  //
            (ES, "Jugador"), //
            (FR, "Joueur"));
  TRANSLATE(greeting,     //
            (EN, "Hi"),   //
            (ES, "Hola"), //
            (FR, "Bonjour"));
  TRANSLATE(farewell,      //
            (EN, "Bye"),   //
            (ES, "Adios"), //
            (FR, "Au revoir"));
}
