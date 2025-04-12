#ifndef STRUCTS_H
#define STRUCTS_H

#include "allegro_libraries.h"
#include "macros.h"
#include "types.h"

typedef struct posicion Posicion;
typedef struct personaje Personaje;
typedef struct boton Boton; 
typedef struct menu Menu; 
typedef struct eventos Eventos;
typedef struct recursos Recursos;

struct posicion
{
    double x;
    double y;
};

struct personaje
{
    Posicion posicion;
};

struct boton 
{ 
    Posicion coordenada_inicial; 
    Posicion coordenada_final; 
}; 

struct menu 
{ 
    ALLEGRO_BITMAP* fondo; 
    ALLEGRO_FONT* fuente; 
    Boton opciones[NRO_OPCIONES_MENU];
    Natural opcion_en_hover;
}; 

struct eventos
{
    ALLEGRO_EVENT_SOURCE* ventana;
    ALLEGRO_EVENT_SOURCE* temporizador;
    ALLEGRO_EVENT_SOURCE* teclado;
    ALLEGRO_EVENT_SOURCE* raton;
};

struct recursos
{
    ALLEGRO_DISPLAY* ventana;
    ALLEGRO_EVENT_QUEUE* cola_eventos;
    ALLEGRO_TIMER* temporizador;
    Eventos eventos;
    Menu menu;
    /* 
    ALLEGRO_FONT* fuentes[6];  // Seran 2 posibles fuentes con 3 tamaños predeterminados cada una -> Se implementara la idea mas adelante
    Lista bitmaps;  // Bitmaps en una lista enlazada (para dinamismo) -> Se implementara la idea mas adelante
    */
};

#endif // STRUCTS_H