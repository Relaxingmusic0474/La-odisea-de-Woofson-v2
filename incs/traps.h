#ifndef TRAPS_H
#define TRAPS_H

#include "allegro_libraries.h"
#include "structs.h"
#include "macros.h"
#include "types.h"
#include "characters.h"
#include "load.h"

// Funciones para manejar trampas (rayos en este caso)
Procedure inicializar_rayo(Rayo*, EfectoSonido*);
Procedure inicializar_rayos(Rayo [NRO_NIVELES][MAX_RAYOS], EfectoSonido*);
Natural detectar_rayos(Mapa, Rayo [], Natural);
Procedure dibujar_rayo(Rayo, ALLEGRO_COLOR);
bool verificar_colision_rayo(Personaje* personaje, Rayo* rayo);
bool linea_de_vision_libre(Rayo, Personaje, Mapa);
bool personaje_activa_rayo(Rayo rayo, Personaje personaje, Mapa);
Procedure actualizar_rayo(Rayo*, Natural, Personaje, Mapa);

#endif // TRAPS_H