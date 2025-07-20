#ifndef LOAD_H
#define LOAD_H

#include "allegro_libraries.h"
#include "characters.h"
#include "enums.h"
#include "structs.h"
#include "types.h"
#include "stdio.h"

ALLEGRO_FONT* cargar_fuente(Fuente, Natural);
Musica* cargar_musica(TipoAudio, ALLEGRO_MIXER*);
EfectoSonido* cargar_efecto_sonido(TipoEfecto, ALLEGRO_MIXER*);
bool cambiar_musica(Recursos*, Musica*);
FILE* cargar_mapa(Natural);
Natural obtener_nro_filas(Natural);
Natural obtener_nro_columnas(Natural);
Mapa leer_mapa(Natural/*, Natural*/);
Procedure dibujar_mapa(Mapa, Imagen [NRO_BLOQUES], Imagen);
Procedure liberar_mapa(Mapa*);
Procedure liberar_mapas(Mapa[]);
bool cargar_escenarios(Recursos*);  // Carga el fondo y el mapa asociado a cada nivel
Procedure liberar_escenarios(Recursos*);
Imagen* cargar_frames(TipoPersonaje);
bool cargar_todos_los_frames(Imagen* [TIPOS_PERSONAJES]);
Procedure destruir_frames(Imagen*, Natural);
Procedure destruir_todos_los_frames(Imagen* [TIPOS_PERSONAJES]);

#endif  // LOAD_H