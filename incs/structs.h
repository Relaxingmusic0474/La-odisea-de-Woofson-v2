#ifndef STRUCTS_H
#define STRUCTS_H

#include "allegro_libraries.h"
#include "macros.h"
#include "types.h"
#include "enums.h"

typedef struct vector Vector;
typedef struct rectangulo Rectangulo;
typedef struct salto Salto;
typedef struct mapa Mapa;
typedef struct personaje Personaje;
typedef struct colision Colision;
typedef struct boton Boton; 
typedef struct menu Menu; 
typedef struct musica Musica;
typedef struct eventos Eventos;
typedef struct recursos Recursos;

struct vector
{
    float x;
    float y;
};

struct rectangulo
{
    Vector pos_inicial;
    Vector pos_final;
};

struct salto  // ANALIZAR SI SERÁ NECESARIO AGREGAR LA POSICIÓN FINAL DEL SALTO A LA ESTRUCTURA
{
    float impulso;  // Impulso que se le da al personaje al saltar
    float tiempo_en_salto;  // Tiempo que lleva en el salto
    float altura_inicial;  // Altura inicial del salto (útil sobre todo si salta desde una plataforma)
    bool en_salto;  // Si el personaje está en salto o no
    bool es_interrumpido;  // Si el salto es interrumpido por la colisión con algún techo o no
    Entero altura_choque;  // Este atributo cobra sentido si el salto es interrumpido por un techo
};

struct mapa
{
    Natural** mapa;
    Natural nro_filas;
    Natural nro_columnas;
    float ancho_bloque;
    float alto_bloque;
};

struct personaje
{
    char tipo;
    Imagen imagen;
    Natural nro_vidas;
    Natural subvida_actual;
    Vector posicion;
    Vector velocidad;
    float ancho;
    float alto;
    float escala_dibujo;  // Escala de dibujo del personaje (para que se vea más grande o más pequeño)
    int bandera_dibujo;  // Si se dibuja normal o en espejo
    bool en_plataforma;  // Si el personaje está en una plataforma o no (si está en el suelo es false)
    Salto salto;
    Vector colision;  // Vector de colisión (para detectar colisiones con el mapa)
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
    bool finalizado;
}; 

struct musica
{
    Audio musica;
    InstanciaAudio instancia;
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
    Personaje pje_principal;
    Ventana ventana;
    ALLEGRO_EVENT_QUEUE* cola_eventos;
    ALLEGRO_TIMER* temporizador;
    Eventos eventos;
    Mapa mapas[NRO_NIVELES];
    Menu menus[NRO_MENUS];
    Menu menu_actual;
    Musica* musicas[NRO_MUSICAS];
    Musica* musica_actual;
    ALLEGRO_VOICE* voz;
    ALLEGRO_MIXER* mixer;
    ALLEGRO_FONT* fuentes[NRO_FUENTES];
    ALLEGRO_BITMAP* vida;
    /* 
    Lista bitmaps;  // Bitmaps en una lista enlazada (para dinamismo) -> Se implementara la idea mas adelante
    */
};

#endif // STRUCTS_H