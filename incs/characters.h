#ifndef CHARACTERS_H
#define CHARACTERS_H

#include "stdio.h"
#include "stdbool.h"
#include "allegro_libraries.h"
#include "macros.h"
#include "enums.h"
#include "structs.h"
#include "types.h"
#include "collisions.h"

bool inicializar_personaje(Personaje*, char);
Procedure inicializar_salto(Personaje*);
Procedure determinar_como_dibujar_personaje(Personaje*, Natural);
Procedure dibujar_personaje(Personaje, Natural);
Procedure actualizar_frame(Personaje*, char);
bool es_posible_mover_personaje_lateralmente(Personaje*, Mapa);
Procedure mover_personaje(Personaje*, Mapa);
Procedure continuar_salto(Personaje*, float, Mapa);
Procedure activar_caida_libre(Personaje*);
Entero velocidad_instantanea(Personaje, float);
bool es_tecla_lateral(Tecla);
Procedure patalear(Personaje*, int);
Procedure detectar_si_personaje_en_zona_de_rayo(Personaje*, Rayo [MAX_RAYOS]);

#endif  // CHARACTERS_H