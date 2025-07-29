#ifndef ALLEGRO_H
#define ALLEGRO_H

#include "allegro_libraries.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "stdbool.h"
#include "math.h"
#include "types.h"
#include "structs.h"
#include "macros.h"
#include "menu.h"
#include "enums.h"
#include "characters.h"
#include "traps.h"

extern bool teclas[ALLEGRO_KEY_MAX];

bool inicializar_allegro();
bool crear_recursos_allegro(Recursos*);
bool inicializar_todo(Recursos*);
Procedure finalizar_allegro(Recursos*);

#endif  // ALLEGRO_H