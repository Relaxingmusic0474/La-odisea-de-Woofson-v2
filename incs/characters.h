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
Procedure determinar_como_dibujar_personaje(Personaje*, bool [ALLEGRO_KEY_MAX], Natural);
Procedure dibujar_personaje(Personaje, bool [ALLEGRO_KEY_MAX], Natural);
Procedure mover_personaje(Personaje*, bool [ALLEGRO_KEY_MAX], Mapa);
Procedure continuar_salto(Personaje*, float, Mapa);
Entero velocidad_instantanea(Personaje, float);
bool es_tecla_lateral(Tecla);

#endif  // CHARACTERS_H