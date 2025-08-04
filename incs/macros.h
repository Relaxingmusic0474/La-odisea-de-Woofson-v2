#ifndef MACROS_H
#define MACROS_H

#include "allegro5/allegro_color.h"  // Para las macros de los colores (lineas 49 a 57)
#include "stdlib.h"

// Macros de funciones básicas útiles
#define X_ENTRE_A_Y_B(x, A, B) (x >= A && x < B)
#define NRO_ELEMS(array) (sizeof(array)/sizeof(array[0]))

// Macros generales
#define NOMBRE_JUEGO "La odisea de Woofson"
#define FPS 60  // Frecuencia de actualización de la pantalla 
#define g 1050  // Gravedad en px/s^2
#define ANCHO_VENTANA 1620
#define ALTO_VENTANA 900
#define ALTURA_PISO (ALTO_VENTANA * 5/6)  // Altura del piso en px
#define ALTO_JUEGO (8./9  * ALTO_VENTANA)
#define RECTANGULO_JUEGO (Rectangulo) {{0, 0}, {ANCHO_VENTANA, ALTO_JUEGO}}
#define RECTANGULO_VENTANA (Rectangulo) {{0, 0}, {ANCHO_VENTANA, ALTO_VENTANA}}
#define RECTANGULO_DATOS (Rectangulo) {{0, ALTO_JUEGO}, {ANCHO_VENTANA, ALTO_VENTANA}}
#define RECTANGULO_MENU_RESULTADO (obtener_rectangulo(RECTANGULO_JUEGO, 500, 1200, 50.0, 50.0))
#define VECTOR_NULO (Vector) {0, 0}
#define NRO_NIVELES 5
#define MAX_DATOS 10
#define MAX_POCIONES 2
#define MAX_POCIONES_RANGO_BALA 3
#define MAX_MUNICIONES 6
#define NRO_COLORES 5
#define NRO_BLOQUES 2 // Tipos distintos de bloques que se cargarán
#define NRO_MENUS 5
#define NRO_ESTADOS 2
#define NRO_MUSICAS 3
#define NRO_INSTANCIAS 8
#define NRO_FUENTES 6
#define NRO_FRAMES_MOVIMIENTO 10
#define NRO_FRAMES_PELEA (NRO_FRAMES_MOVIMIENTO)
#define NRO_FRAMES_DISPARO (NRO_FRAMES_MOVIMIENTO)
#define MAX_ENEMIGOS 10

#define MAX_BALAS 5
#define MAX_TIEMPO_BALA_MAXIMO_ALCANCE 25
#define MAX_FRAMES_ESPERA_ENEMIGO 80
#define MAX_FRAMES_ESPERA_WOOFSON 15
#define MAX_FRAMES_ESPERA_RECARGA 240
#define MAX_FRAMES_ESPERA_DRAGON 100
#define PUNTUACION_INICIAL 30
#define VELOCIDAD_BALA 9
#define RADIO_AXIAL_X_BALA 6
#define RADIO_AXIAL_Y_BALA 4
#define RADIO_AXIAL_X_BALA_PRO 9
#define RADIO_AXIAL_Y_BALA_PRO 7
#define RANGO_BALA_WOOFSON (ANCHO_VENTANA * 0.25)
#define RANGO_BALA_PRO_WOOFSON (ANCHO_VENTANA * 0.40)
#define DANHO_BALA_EXTRATERRESTRE 15
#define DANHO_BALA_WOOFSON 34
#define DANHO_FUEGO 57
#define NRO_FRAMES_WOOFSON (NRO_FRAMES_MOVIMIENTO + NRO_FRAMES_PELEA + NRO_FRAMES_DISPARO)
#define NRO_FRAMES_DRAGON 10
#define NRO_FRAMES_EXTRATERRESTRE 10
#define NRO_FRAMES_MONSTRUO 13

// Macros para los elementos del mapa
#define NADA 0  // Representa un espacio vacío en el mapa
#define BLOQUE 1  // Representa un bloque del mapa (un bloque sólido)
#define BLOQUE_RAYO 2  // Representa una trampa del mapa (un bloque del cual sale una trampa)
#define ESPINA 3  // Una espina que dañará al personaje
#define EXTRATERRESTRE_ESTATICO 4  // Un extraterrestre que no se mueve
#define EXTRATERRESTRE_DINAMICO 5  // Un extraterrestre que sí se moverá
#define _DRAGON 6  // _DRAGON en vez de DRAGON para que no se confunda con el enum
#define _MONSTRUO 7  // Caso similar
#define PUERTA 8  // Puerta para pasar de nivel
#define PALANCA 9  // Palanca para abrir o cerrar la puerta
#define CHARCO 10  // Otro tipo de trampa
#define POCION 11  // Poción para aumentar la vida de Woofson
#define MUNICION 12  // Para que Woofson recoja municiones cuando le falten
#define POCION_RANGO_BALA 13  // Poción que servirá para aumentar el rango de bala de Woofson.

// Macros para las caracteristicas del personaje
#define VELOCIDAD_MAXIMA_PERSONAJE 8
#define MAX_TIEMPO_INMUNE_WOOFSON 1.5  // Tiempo en que Woofson no puede recibir un daño adicional
#define MAX_TIEMPO_INMUNE_ENEMIGO 1.0
#define TIEMPO_MUERTE 2.0  // Tiempo que se tomará Woofson en morir, para volver a comenzar desde su posicion inicial
#define ACELERACION_PERSONAJE 0.2  // Aceleración del personaje al caminar
#define VELOCIDAD_DRAGONES 2
#define VELOCIDAD_EXTRATERRESTRES 3
#define VELOCIDAD_MONSTRUO 7
#define ACELERACION_ADREDE 5 // Aceleración adicional que se genera al presionar la tecla de abajo estando en caida libre
#define IMPULSO_PERSONAJE 700  // Impulso de salto del personaje en px/s

#define TIEMPO_MUERTE_ENEMIGO 5.0  // Durante 5 segundos el enemigo muerto no vuelve a aparecer

// Macros para los rayos
#define MAX_RAYOS 8  // Número máximo de rayos que se pueden detectar en el mapa
#define NRO_OSCILACIONES_RAYO 10  // Número de oscilaciones que tendrá el rayo
#define RADIO_CIRCULO_ROJO 5  // Radio del círculo que representará el radio de los círculos de donde se originan los rayos
#define GROSOR_INICIAL_RAYO 4  // Grosor del rayo
#define DANHO_RAYO 28  // Daño que causa el rayo al impactar al personaje
#define TIEMPO_RAYO_EN_ESPERA 0.5  // Tiempo en segundos que el rayo estará inactivo antes de volver a activarse
#define TIEMPO_RAYO_EN_APARICION 0.4  // Tiempo en segundos que el rayo demorará en aparecer por completo
#define TIEMPO_RAYO_ACTIVO_AL_100 0.3  // Tiempo en segundos que el rayo estará activo al 100% de su longitud
#define TIEMPO_RAYO_EN_DESAPARICION (TIEMPO_RAYO_EN_APARICION)  // Tiempo en segundos que el rayo demorará en desaparecer por completo
#define TIEMPO_CICLO_RAYO (TIEMPO_RAYO_EN_ESPERA + TIEMPO_RAYO_EN_APARICION + TIEMPO_RAYO_ACTIVO_AL_100 + TIEMPO_RAYO_EN_DESAPARICION) // Tiempo total del ciclo del rayo
#define TOLERANCIA 1e-2  // Tolerancia para las comparaciones de punto flotante

// Macros para las espinas
#define NRO_REGRESIONES 2
#define FACTOR_ESPINA 0.5

// Macro que define la cantidad de vidas que tendrá el personaje inicialmente
#define VIDAS_INICIALES 3

// Macros para cargar fuentes de distinto tamaño
#define NORMAL 20
#define GRANDE 24
#define GIGANTE 36

// Macros para los largos de cadena
#define LARGO 35
#define MAXLINEA 128

// Macros para los colores
#define ROJO al_map_rgb(255, 0, 0)
#define VERDE al_map_rgb(0, 255, 0)
#define VERDE_OSCURO al_map_rgb(0, 215, 0)
#define AZUL al_map_rgb(0, 0, 255)
#define BLANCO al_map_rgb(255, 255, 255)
#define NEGRO al_map_rgb(0, 0, 0)
#define GRIS al_map_rgb(205, 205, 205)
#define GRIS_SEMIOSCURO al_map_rgb(185, 185, 185)
#define GRIS_OSCURO al_map_rgb(130, 137, 143)
#define CAFE al_map_rgb(111, 78, 55)
#define AMARILLO al_map_rgb(255, 255, 0)
#define ROSADO al_map_rgb(216, 84, 180)
#define MORADO al_map_rgb(128, 0, 128)
#define NARANJO al_map_rgb(255, 165, 0)
#define TURQUESA al_map_rgb(93, 193, 185)

// Macros de estilo personal de programacion (para hacerlo mas legible para mi)
#define LOOP while (1)
#define MAPA_INVALIDO (Mapa) {NULL, 0, 0, 0., 0.}

#endif // MACROS_H