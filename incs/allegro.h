#ifndef ALLEGRO_H
#define ALLEGRO_H

#include "allegro_libraries.h"
#include "stdio.h"
#include "stdbool.h"
#include "types.h"
#include "structs.h"
#include "macros.h"
#include "menu.h"
#include "enums.h"

bool inicializacion_allegro();
bool creacion_recursos_allegro(Recursos*);
Procedure finalizacion_allegro(Recursos*);

#endif  // ALLEGRO_H