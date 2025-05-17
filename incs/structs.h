#ifndef STRUCTS_H
#define STRUCTS_H

#include "allegro_libraries.h"
#include "macros.h"
#include "types.h"
#include "enums.h"

typedef struct posicion Posicion;
typedef struct personaje Personaje;
typedef struct boton Boton; 
typedef struct menu Menu; 
typedef struct musica Musica;
typedef struct eventos Eventos;
typedef struct recursos Recursos;

struct posicion
{
    double x;
    double y;
};

struct personaje
{
    Imagen imagen;
    Posicion posicion;
    float velocidad;
    float ancho;
    float alto;
    char tipo;  // Si es malo o bueno
    bool inicializado;
    int bandera_dibujo;  // Si se dibuja normal o en espejo
    // bool en_movimiento;
};

struct boton 
{ 
    Posicion coordenada_inicial; 
    Posicion coordenada_final;
    char texto[15]; 
}; 

struct menu 
{ 
    Imagen fondo; 
    ALLEGRO_FONT* fuente; 
    Boton* opciones;
    Natural nro_opciones;
    Natural opcion_en_hover;
    bool inicializado;
}; 

struct musica
{
    Audio musica;
    ALLEGRO_SAMPLE_ID ID;
    TipoAudio tipo;
    // ALLEGRO_VOICE* voz;
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
    Ventana ventana;
    ALLEGRO_EVENT_QUEUE* cola_eventos;
    ALLEGRO_TIMER* temporizador;
    Eventos eventos;
    /* 
    ALLEGRO_FONT* fuentes[6];  // Seran 2 posibles fuentes con 3 tamaños predeterminados cada una -> Se implementara la idea mas adelante
    Lista bitmaps;  // Bitmaps en una lista enlazada (para dinamismo) -> Se implementara la idea mas adelante
    */
};

#endif // STRUCTS_H