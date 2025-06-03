#ifndef LOAD_H
#define LOAD_H

#include "allegro_libraries.h"
#include "enums.h"
#include "structs.h"
#include "types.h"
#include "stdio.h"

ALLEGRO_FONT* cargar_fuente(Fuente, Natural);
Musica* cargar_musica(TipoAudio);
FILE* cargar_mapa(Natural);
Natural obtener_nro_filas(Natural);
Natural obtener_nro_columnas(Natural);
Natural** leer_mapa(Natural, Natural*, Natural*);
Procedure dibujar_mapa(Natural* [], Natural, Natural);
Procedure liberar_mapa(Natural* [], Natural);

#endif  // LOAD_H