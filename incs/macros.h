#ifndef MACROS_H
#define MACROS_H

#include "allegro5/allegro_color.h"  // Para las macros de los colores (lineas 17 a 21)

// Macros generales
#define FPS 60
#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 600
#define NRO_NIVELES 5

// Macros para cargar fuentes de distinto tamaño
#define NORMAL 14
#define GRANDE 22
#define GIGANTE 36

// Macros para los colores
#define ROJO al_map_rgb(255, 0, 0)
#define VERDE al_map_rgb(0, 255, 0)
#define AZUL al_map_rgb(0, 0, 255)
#define BLANCO al_map_rgb(255, 255, 255)
#define NEGRO al_map_rgb(0, 0, 0)

// Macros para los comandos
#define COMANDO_COMPILACION_PROGRAMA_PRUEBA "gcc prueba_allegro.c allegro.o load.o menu.o -lallegro -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec -lallegro_image -lallegro_color -o prueba_allegro"
#define COMANDO_EJECUCION_PROGRAMA_PRUEBA "./prueba_allegro"
#define COMANDO_COMPILACION_JUEGO "gcc woofson.c allegro.o load.o menu.o -lallegro -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec -lallegro_image -lallegro_color -o woofson"
#define COMANDO_EJECUCION_JUEGO "./woofson"

// Macros para estados
#define EXITOSA 0

// Macros de estilo personal de programacion (para hacerlo mas legible para mi)
#define LOOP while (1)

#endif // MACROS_H