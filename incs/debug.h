#ifndef DEBUG_H
#define DEBUG_H

#include "allegro.h"
#include "stdlib.h"
#include "time.h"
#include "enums.h"

Procedure mostrar_mapa(Mapa);
Procedure mostrar_datos_personaje(Personaje);
Procedure mostrar_bloque_actual_personaje(Personaje, Mapa);
Procedure caida_libre_activada(Personaje);
Procedure debug_rayo_personaje(Rayo*, Personaje*);
Procedure imprimir_datos_rayos(Rayo [MAX_RAYOS], Natural);
Procedure imprimir_posicion_balas(Bala [MAX_BALAS]);
Procedure mostrar_estados_actuales(Etapa, Natural);
Procedure imprimir_posicion_charcos(Charco [MAX_CHARCOS]);

#endif  // DEBUG_H