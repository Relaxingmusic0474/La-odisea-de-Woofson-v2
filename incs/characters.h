#ifndef CHARACTERS_H
#define CHARACTERS_H

#include "stdio.h"
#include "stdbool.h"
#include "allegro_libraries.h"
#include "macros.h"
#include "enums.h"
#include "structs.h"
#include "types.h"
#include "score.h"
#include "collisions.h"

TipoFrame tipo_frame(TipoPersonaje);
TipoPersonaje tipo_personaje(TipoFrame);
Procedure inicializar_personaje(Personaje*, TipoPersonaje, Imagen* [TIPOS_PERSONAJES], Vector, bool);
Procedure inicializar_salto(Personaje*);
Procedure determinar_como_dibujar_personaje(Personaje*, Natural);
Procedure dibujar_personaje(Personaje*, Natural, Natural);
Procedure actualizar_frame(Personaje*, ModoWoofson, Natural);
bool es_posible_mover_personaje_lateralmente(Personaje*, Mapa);
Procedure mover_personaje(Personaje*, Mapa, Natural);
Procedure mover_enemigo_dinamico(Personaje*, Personaje, Mapa, Natural);
Procedure continuar_salto(Personaje*, float, Mapa);
Procedure activar_caida_libre(Personaje*);
Entero velocidad_instantanea(Personaje, float);
bool es_tecla_lateral(Tecla);
Procedure patalear(Personaje*, int);
Procedure morir(Personaje*, Tecla*, Etapa*);
Procedure determinar_victoria_woofson(Personaje*, Personaje [MAX_ENEMIGOS], Puerta, Etapa*);
Procedure aplicar_danho(Personaje*, Natural);
Procedure aumentar_subvida(Personaje*, Natural);
Procedure actualizar_estado_danho(Personaje*);
Procedure actualizar_estado_danho_enemigos(Personaje [MAX_ENEMIGOS]);
Procedure detectar_si_personaje_en_zona_de_rayo(Personaje*, Rayo [MAX_RAYOS]);
bool puede_disparar_horizontalmente(Personaje, Personaje, Mapa);
bool hay_balas_activas(Bala [MAX_BALAS]);
Natural nro_balas_disponibles(Bala [MAX_BALAS]);
Procedure mover_balas_activas(Personaje*, Personaje*, Mapa, Natural, ALLEGRO_COLOR);
bool puede_disparar_horizontalmente(Personaje, Personaje, Mapa);
Procedure efectuar_disparo_de_enemigo(Personaje*, Personaje*, Mapa, Natural);
Procedure efectuar_disparo_de_enemigos(Personaje [MAX_ENEMIGOS], Personaje*, Mapa, Natural, Imagen);
Natural nro_enemigos_activos(Personaje [MAX_ENEMIGOS]);
bool woofson_puede_disparar(Personaje*);
Procedure efectuar_disparo_de_woofson(Personaje*, Personaje [MAX_ENEMIGOS], Mapa, Natural);
Procedure desactivar_enemigos(Personaje [MAX_ENEMIGOS]);

#endif  // CHARACTERS_H