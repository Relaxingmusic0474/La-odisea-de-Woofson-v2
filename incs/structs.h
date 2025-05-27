#ifndef STRUCTS_H
#define STRUCTS_H

#include "allegro_libraries.h"
#include "macros.h"
#include "types.h"
#include "enums.h"

typedef struct vector Vector;
typedef struct salto Salto;
typedef struct personaje Personaje;
typedef struct colision Colision;
typedef struct boton Boton; 
typedef struct menu Menu; 
typedef struct musica Musica;
typedef struct eventos Eventos;
typedef struct recursos Recursos;

struct vector
{
    Entero x;
    Entero y;
};

struct salto  // ANALIZAR SI SERÁ NECESARIO AGREGAR LA POSICIÓN FINAL DEL SALTO A LA ESTRUCTURA
{
    float impulso; // Impulso que se le da al personaje al saltar
    float tiempo_en_salto; // Tiempo que lleva en el salto
    float altura_inicial; // Altura inicial del salto (útil sobre todo si salta desde una plataforma)
    bool en_salto; // Si el personaje está en salto o no
    bool es_interrumpido; // Si el salto es interrumpido por la colisión con algún techo o no
    Entero altura_choque; // Este atributo cobra sentido si el salto es interrumpido por un techo
};

struct personaje
{
    Imagen imagen;
    Vector posicion;
    Vector velocidad;
    // Vector aceleracion;  // Aceleración del personaje (puede ser útil para implementar la gravedad o un MRUV)
    // float impulso;  // Este es el impulso que se le da al personaje al saltar
    float ancho;
    float alto;
    float escala_dibujo;  // Escala de dibujo del personaje (para que se vea más grande o más pequeño)
    int bandera_dibujo;  // Si se dibuja normal o en espejo
    char tipo;  // Si es malo o bueno
    // bool inicializado;  // Este atributo puede que NO sea necesario (se podria eliminar)
    
    // Estos atributos ahora se reemplazarán por una variable estructura Salto
    Salto salto;
    // bool en_salto;
    // float tiempo_salto;  // Tiempo que lleva en el salto
    // bool en_movimiento;
};

struct boton 
{ 
    Vector coordenada_inicial; 
    Vector coordenada_final;
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