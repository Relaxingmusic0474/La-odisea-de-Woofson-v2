#ifndef MACROS_H
#define MACROS_H

#include "allegro5/allegro_color.h"  // Para las macros de los colores (lineas 49 a 57)

// Macros generales
#define NOMBRE_JUEGO "La odisea de Woofson"
#define FPS 60  // Frecuencia de actualización de la pantalla 
#define g 1050  // Gravedad en px/s^2
#define ANCHO_VENTANA 1620
#define ALTO_VENTANA 900
#define ALTURA_PISO (ALTO_VENTANA * 5/6)  // Altura del piso en px
#define NRO_NIVELES 5
#define NRO_MENUS 2
#define NRO_MUSICAS 2
#define NRO_FUENTES 6
#define NRO_FRAMES_MOVIMIENTO 10
#define NRO_FRAMES_PELEA (NRO_FRAMES_MOVIMIENTO)
#define NRO_FRAMES 20  // Número de frames que tendrá cada personaje
#define MAX_RAYOS 8  // Número máximo de rayos que se pueden detectar en el mapa

// Macros para los elementos del mapa
#define NADA 0  // Representa un espacio vacío en el mapa
#define BLOQUE 1  // Representa un bloque del mapa (un bloque sólido)
#define TRAMPA 2  // Representa una trampa del mapa (un bloque del cual sale una trampa)

// Macros para las velocidades de los personajes
#define VELOCIDAD_MAXIMA_PERSONAJE 8
#define ACELERACION_PERSONAJE 0.2  // Aceleración del personaje al caminar
#define VELOCIDAD_DRAGONES 4
#define VELOCIDAD_HUMANOIDES 3
#define VELOCIDAD_JEFE 7
#define ACELERACION_ADREDE 5 // Aceleración adicional que se genera al presionar la tecla de abajo estando en caida libre

// Macro para definir los daños que causan las trampas o los enemigos al personaje
#define DANHO_RAYO 28

// Macro que define la cantidad de vidas que tendrá el personaje inicialmente
#define VIDAS_INICIALES 5

// Macros para manejar cosas relacionadas con el salto del personaje
#define IMPULSO_PERSONAJE 700  // Impulso de salto del personaje en px/s

// Macros para cargar fuentes de distinto tamaño
#define NORMAL 20
#define GRANDE 24
#define GIGANTE 36

// Macros para los largos de cadena
#define LARGO 25
#define MAXLINEA 128

// Macros para los colores
#define ROJO al_map_rgb(255, 0, 0)
#define VERDE al_map_rgb(0, 255, 0)
#define VERDE_OSCURO al_map_rgb(0, 215, 0)
#define AZUL al_map_rgb(0, 0, 255)
#define BLANCO al_map_rgb(255, 255, 255)
#define NEGRO al_map_rgb(0, 0, 0)
#define GRIS al_map_rgb(205, 205, 205)
#define CAFE al_map_rgb(111, 78, 55)
#define AMARILLO al_map_rgb(255, 255, 0)

// Macros de estilo personal de programacion (para hacerlo mas legible para mi)
#define LOOP while (1)
#define MAPA_INVALIDO (Mapa) {NULL, 0, 0, 0., 0.}

#endif // MACROS_H