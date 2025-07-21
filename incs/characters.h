#ifndef CHARACTERS_H
#define CHARACTERS_H

#include "stdio.h"
#include "stdbool.h"
#include "dirent.h"
#include "allegro_libraries.h"
#include "macros.h"
#include "enums.h"
#include "structs.h"
#include "types.h"
#include "collisions.h"

TipoFrame tipo_frame(TipoPersonaje);
TipoPersonaje tipo_personaje(TipoFrame);
Procedure inicializar_personaje(Personaje*, TipoPersonaje, Imagen* [TIPOS_PERSONAJES], Vector, bool);
Procedure inicializar_salto(Personaje*);
Procedure determinar_como_dibujar_personaje(Personaje*, Natural);
Procedure dibujar_personaje(Personaje, Natural, Natural);
Procedure actualizar_frame(Personaje*, char);
bool es_posible_mover_personaje_lateralmente(Personaje*, Mapa);
Procedure mover_personaje(Personaje*, Mapa);
Procedure mover_enemigo_dinamico(Personaje*, Mapa);
Procedure continuar_salto(Personaje*, float, Mapa);
Procedure activar_caida_libre(Personaje*);
Entero velocidad_instantanea(Personaje, float);
bool es_tecla_lateral(Tecla);
Procedure patalear(Personaje*, int);
Procedure morir(Personaje*, Tecla*, Etapa*);
Procedure aplicar_danho(Personaje*, Natural);
Procedure aumentar_subvida(Personaje*, Natural);
Procedure detectar_si_personaje_en_zona_de_rayo(Personaje*, Rayo [MAX_RAYOS]);

#endif  // CHARACTERS_H