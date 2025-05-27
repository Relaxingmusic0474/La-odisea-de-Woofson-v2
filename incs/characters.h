#ifndef CHARACTERS_H
#define CHARACTERS_H

#include "allegro.h"

bool inicializar_personaje(Personaje*, char);
Procedure mover_personaje(Personaje*, bool [ALLEGRO_KEY_MAX]);
bool hay_colision_con_bordes(Personaje);
bool hay_colision_izquierda(Personaje);
bool hay_colision_derecha(Personaje);
bool hay_colision_superior(Personaje);
bool hay_colision_inferior(Personaje);
Procedure efectuar_colision_con_bordes(Personaje*);
Procedure continuar_salto(Personaje*, float);
Entero velocidad_instantanea(Personaje, float);
bool es_tecla_lateral(Tecla);

#endif  // CHARACTERS_H