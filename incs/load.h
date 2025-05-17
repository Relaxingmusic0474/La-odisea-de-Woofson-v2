#ifndef LOAD_H
#define LOAD_H

#include "allegro_libraries.h"
#include "enums.h"
#include "structs.h"
#include "types.h"
#include "stdio.h"

ALLEGRO_FONT* cargar_fuente(Fuente, Natural);
Musica* cargar_musica(TipoAudio);

#endif  // LOAD_H