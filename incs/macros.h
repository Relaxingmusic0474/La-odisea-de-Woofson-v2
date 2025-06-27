#ifndef MACROS_H
#define MACROS_H

#include "allegro5/allegro_color.h"  // Para las macros de los colores (lineas 20 a 25)

// Macros generales
#define NOMBRE_JUEGO "La odisea de Woofson"
#define FPS 60
#define g 1050 // Gravedad en px/s^2
#define ANCHO_VENTANA 1620 // 1080
#define ALTO_VENTANA 900 // 600
#define ALTURA_PISO (ALTO_VENTANA * 5/6) // Altura del piso en px
#define NRO_NIVELES 5
#define NRO_MUESTRAS_AUDIO_RESERVADAS 4

// Macros para las velocidades de los personajes
#define VELOCIDAD_PERSONAJE 5
#define VELOCIDAD_DRAGONES 4
#define VELOCIDAD_HUMANOIDES 3
#define VELOCIDAD_JEFE 7

// Macros para manejar cosas relacionadas con el salto del personaje
#define IMPULSO_PERSONAJE 700  /* Impulso de salto del personaje en px/s */
#define EPSILON 8  /* Tolerancia para evitar caidas al inframundo del personaje */

// Macros para cargar fuentes de distinto tamaño
#define NORMAL 14
#define GRANDE 24
#define GIGANTE 36

// Macros para los largos de cadena
#define LARGO 25
#define MAXLINEA 64

// Macros para los colores
#define ROJO al_map_rgb(255, 0, 0)
#define VERDE al_map_rgb(0, 255, 0)
#define AZUL al_map_rgb(0, 0, 255)
#define BLANCO al_map_rgb(255, 255, 255)
#define NEGRO al_map_rgb(0, 0, 0)
#define GRIS al_map_rgb(205, 205, 205)
#define CAFE al_map_rgb(111, 78, 55)

// Macros de estilo personal de programacion (para hacerlo mas legible para mi)
#define LOOP while (1)
#define MAPA_INVALIDO (Mapa) {NULL, 0, 0}

#endif // MACROS_H