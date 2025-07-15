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
#define NRO_COLORES 5
#define NRO_MENUS 2
#define NRO_MUSICAS 2
#define NRO_INSTANCIAS 8
#define NRO_FUENTES 6
#define NRO_FRAMES_MOVIMIENTO 10
#define NRO_FRAMES_PELEA (NRO_FRAMES_MOVIMIENTO)
#define NRO_FRAMES 20  // Número de frames que tendrá cada personaje

// Macros para los rayos
#define MAX_RAYOS 8  // Número máximo de rayos que se pueden detectar en el mapa
#define NRO_OSCILACIONES_RAYO 10  // Número de oscilaciones que tendrá el rayo
#define RADIO_CIRCULO_ROJO 5  // Radio del círculo que representará el radio de los círculos de donde se originan los rayos
#define GROSOR_RAYO 6  // Grosor del rayo
#define DANHO_RAYO 28  // Daño que causa el rayo al impactar al personaje
#define TIEMPO_RAYO_EN_ESPERA 0.6  // Tiempo en segundos que el rayo estará inactivo antes de volver a activarse
#define TIEMPO_RAYO_EN_APARICION 0.2  // Tiempo en segundos que el rayo demorará en aparecer por completo
#define TIEMPO_RAYO_ACTIVO_AL_100 0.3  // Tiempo en segundos que el rayo estará activo al 100% de su longitud
#define TIEMPO_RAYO_EN_DESAPARICION (TIEMPO_RAYO_EN_APARICION)  // Tiempo en segundos que el rayo demorará en desaparecer por completo
#define TIEMPO_CICLO_RAYO (TIEMPO_RAYO_EN_ESPERA + TIEMPO_RAYO_EN_APARICION + TIEMPO_RAYO_ACTIVO_AL_100 + TIEMPO_RAYO_EN_DESAPARICION) // Tiempo total del ciclo del rayo
#define TOLERANCIA 1e-2  // Tolerancia para las comparaciones de punto flotante
#define PI 3.141593

// Macros para los elementos del mapa
#define NADA 0  // Representa un espacio vacío en el mapa
#define BLOQUE 1  // Representa un bloque del mapa (un bloque sólido)
#define TRAMPA 2  // Representa una trampa del mapa (un bloque del cual sale una trampa)

// Macros para las caracteristicas del personaje
#define VELOCIDAD_MAXIMA_PERSONAJE 6
#define MAX_TIEMPO_INMUNE 2  // Máximo tiempo en que el personaje no puede recibir un daño adicional
#define ACELERACION_PERSONAJE 0.2  // Aceleración del personaje al caminar
#define VELOCIDAD_DRAGONES 4
#define VELOCIDAD_HUMANOIDES 3
#define VELOCIDAD_JEFE 7
#define ACELERACION_ADREDE 5 // Aceleración adicional que se genera al presionar la tecla de abajo estando en caida libre

// Macro para definir los daños que causan las trampas o los enemigos al personaje
#define DANHO_RAYO 28

// Macro que define la cantidad de vidas que tendrá el personaje inicialmente
#define VIDAS_INICIALES 3

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
#define ROSADO al_map_rgb(216, 84, 180)
#define MORADO al_map_rgb(128, 0, 128)

// Macros de estilo personal de programacion (para hacerlo mas legible para mi)
#define LOOP while (1)
#define MAPA_INVALIDO (Mapa) {NULL, 0, 0, 0., 0.}

#endif // MACROS_H