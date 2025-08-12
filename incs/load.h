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
void dibujar_mapa(Mapa, Recursos*, bool*, Natural, Natural);
void liberar_mapa(Mapa*);
void liberar_mapas(Mapa[]);
bool cargar_escenarios(Recursos*);  // Carga el fondo y el mapa asociado a cada nivel
void liberar_escenarios(Recursos*);
Imagen* cargar_frames(TipoPersonaje);
bool cargar_todos_los_frames(Imagen* [TIPOS_PERSONAJES]);
void destruir_frames(Imagen*, Natural);
void destruir_todos_los_frames(Imagen* [TIPOS_PERSONAJES]);

#endif  // LOAD_H