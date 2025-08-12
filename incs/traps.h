#ifndef TRAPS_H
#define TRAPS_H

#include "allegro_libraries.h"
#include "structs.h"
#include "macros.h"
#include "types.h"
#include "characters.h"
#include "load.h"

// Funciones para manejar trampas (rayos en este caso)
void inicializar_rayo(Rayo*, EfectoSonido*);
void inicializar_rayos(Rayo [NRO_NIVELES][MAX_RAYOS], EfectoSonido*);
Natural detectar_rayos(Mapa, Rayo [], Natural);
void dibujar_rayo(Rayo, ALLEGRO_COLOR);
bool verificar_colision_rayo(Personaje* personaje, Rayo* rayo);
bool linea_de_vision_libre(Rayo, Personaje, Mapa);
bool personaje_activa_rayo(Rayo rayo, Personaje personaje, Mapa);
void actualizar_rayo(Rayo*, Natural, Personaje, Mapa);
void actualizar_rayos(Rayo [MAX_RAYOS], Natural, Personaje, Mapa);
void detectar_si_personaje_en_zona_de_espina(Personaje* personaje, Espina espinas[MAX_ESPINAS]);
void detectar_si_personaje_en_zona_de_veneno(Personaje* personaje, Charco charcos[MAX_CHARCOS], Natural);
//bool hay_hitbox_con_espina(Personaje*, Mapa, Espina*);

#endif // TRAPS_H