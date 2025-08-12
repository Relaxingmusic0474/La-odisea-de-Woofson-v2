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
void inicializar_personaje(Personaje*, TipoPersonaje, Imagen* [TIPOS_PERSONAJES], Vector, bool);
void inicializar_salto(Personaje*);
void determinar_como_dibujar_personaje(Personaje*, Natural);
void dibujar_personaje(Personaje*, Natural, Natural);
void actualizar_frame(Personaje*, ModoWoofson, Natural);
bool es_posible_mover_personaje_lateralmente(Personaje*, Mapa);
void mover_personaje(Personaje*, Mapa, Natural);
void mover_enemigo_dinamico(Personaje*, Personaje, Mapa, Natural);
void continuar_salto(Personaje*, float, Mapa);
void activar_caida_libre(Personaje*);
float velocidad_instantanea(Personaje, float);
bool es_tecla_lateral(Tecla);
void patalear(Personaje*, int);
void morir(Personaje*, Tecla*, Etapa*);
void determinar_victoria_woofson(Personaje*, Personaje [MAX_ENEMIGOS], Puerta, Etapa*);
void aplicar_danho(Personaje*, Natural);
void aumentar_subvida(Personaje*, Natural);
void actualizar_estado_danho(Personaje*);
void actualizar_estado_danho_enemigos(Personaje [MAX_ENEMIGOS]);
void detectar_si_personaje_en_zona_de_rayo(Personaje*, Rayo [MAX_RAYOS]);
bool puede_disparar_horizontalmente(Personaje, Personaje, Mapa);
bool hay_balas_activas(Bala [MAX_BALAS]);
Natural nro_balas_disponibles(Bala [MAX_BALAS]);
void mover_balas_activas(Personaje*, Personaje*, Mapa, Natural, ALLEGRO_COLOR);
bool puede_disparar_horizontalmente(Personaje, Personaje, Mapa);
void efectuar_disparo_de_enemigo(Personaje*, Personaje*, Mapa, Natural);
void efectuar_disparo_de_enemigos(Personaje [MAX_ENEMIGOS], Personaje*, Mapa, Natural, Imagen);
Natural nro_enemigos_activos(Personaje [MAX_ENEMIGOS]);
bool woofson_puede_disparar(Personaje*);
void efectuar_disparo_de_woofson(Personaje*, Personaje [MAX_ENEMIGOS], Mapa, Natural);
void desactivar_enemigos(Personaje [MAX_ENEMIGOS]);

#endif  // CHARACTERS_H