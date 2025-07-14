#ifndef TRAPS_H
#define TRAPS_H

#include "allegro_libraries.h"
#include "structs.h"
#include "macros.h"
#include "types.h"
#include "characters.h"
#include "load.h"

// Funciones para manejar trampas (rayos en este caso)
Procedure inicializar_rayo(Rayo*, Musica*);
Procedure inicializar_rayos(Rayo [NRO_NIVELES][MAX_RAYOS], Musica*);
Natural detectar_rayos(Mapa, Rayo [], Natural);
Procedure dibujar_rayo(Rayo, ALLEGRO_COLOR);
bool verificar_colision_rayo(Personaje* personaje, Rayo* rayo);
bool personaje_activa_rayo(Rayo rayo, Personaje personaje);
Procedure actualizar_rayo(Rayo*, Personaje);

#endif // TRAPS_H