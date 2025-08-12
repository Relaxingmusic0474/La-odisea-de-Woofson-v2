#ifndef DEBUG_H
#define DEBUG_H

#include "allegro.h"
#include "stdlib.h"
#include "time.h"
#include "enums.h"

void mostrar_mapa(Mapa);
void mostrar_datos_personaje(Personaje);
void mostrar_bloque_actual_personaje(Personaje, Mapa);
void caida_libre_activada(Personaje);
void debug_rayo_personaje(Rayo*, Personaje*);
void imprimir_datos_rayos(Rayo [MAX_RAYOS], Natural);
void imprimir_posicion_balas(Bala [MAX_BALAS]);
void mostrar_estados_actuales(Etapa, Natural);
void imprimir_posicion_charcos(Charco [MAX_CHARCOS]);

#endif  // DEBUG_H