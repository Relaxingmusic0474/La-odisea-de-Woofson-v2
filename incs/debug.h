#ifndef DEBUG_H
#define DEBUG_H

#include "allegro.h"

Procedure mostrar_mapa(Mapa);
Procedure mostrar_datos_personaje(Personaje);
Procedure mostrar_bloque_actual_personaje(Personaje, Mapa);
Procedure caida_libre_activada(Personaje);
Procedure debug_rayo_personaje(Rayo*, Personaje*);

#endif  // DEBUG_H