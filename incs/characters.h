#ifndef CHARACTERS_H
#define CHARACTERS_H

#include "allegro.h"

bool inicializar_personaje(Personaje*, char);
void mover_personaje(Personaje*, bool [ALLEGRO_KEY_MAX]);
bool es_tecla_lateral(Tecla);

#endif  // CHARACTERS_H