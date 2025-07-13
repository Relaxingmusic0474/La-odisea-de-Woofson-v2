#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "allegro.h"
#include "structs.h"
#include "debug.h"

bool hay_colision_con_bordes(Personaje*, Mapa);
bool hay_colision_superior(Personaje*, Mapa);
bool hay_bloque_arriba(Personaje*, Mapa);
bool hay_colision_inferior(Personaje*, Mapa);
bool hay_bloque_debajo(Personaje*, Mapa);
bool hay_colision_izquierda(Personaje*, Mapa);
bool hay_bloque_izquierda(Personaje*, Mapa);
bool hay_colision_derecha(Personaje*, Mapa);
bool hay_bloque_derecha(Personaje*, Mapa);
Procedure efectuar_colision(Personaje*, Mapa);

#endif // COLLISIONS_H