#ifndef STRUCTS_H
#define STRUCTS_H

#include "allegro_libraries.h"
#include "macros.h"
#include "types.h"
#include "enums.h"

typedef struct vector Vector;
typedef struct rectangulo Rectangulo;
typedef struct salto Salto;
typedef struct rayo Rayo;
typedef struct mapa Mapa;
typedef struct personaje Personaje;
typedef struct colision Colision;
typedef struct boton Boton; 
typedef struct menu Menu; 
typedef struct musica Musica;
typedef struct efectoSonido EfectoSonido;
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
    Imagen frames[NRO_FRAMES];  // Arreglo de imágenes para las animaciones del personaje
    Imagen imagen;  // Imagen actual del personaje
    Natural id_nro_frame; // Número del frame actual del personaje (para las animaciones)
    Natural nro_vidas;
    Natural subvida_actual;
    Vector posicion;
    Vector velocidad;
    float ancho;
    float alto;
    int bandera_dibujo;  // Si se dibuja normal o en espejo
    bool en_plataforma;  // Si el personaje está en una plataforma o no (si está en el suelo es false)
    bool caminata;  // Si el personaje está en movimiento o intentando moverse (caminando, corriendo o pataleando)
    unsigned int fps_en_caminata;  // Número de frames en caminata (para la animación)
    unsigned int fps_en_pelea; // Número de frames en pelea (para la animación)
    Salto salto;
    bool danhado;
    // CREAR UN ARREGLO DE MUNICIONES
    
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
};

struct efectoSonido
{
    Audio musica;
    InstanciaAudio instancias[NRO_INSTANCIAS];
    TipoEfecto tipo;
};

struct rayo  // Tipo de trampa que se usa en el juego
{
    Vector origen;  // Punto de origen del rayo
    Vector objetivo;  // Posición a la que apunta el rayo
    Vector posicion;  // Posición actual del rayo
    Vector velocidad;  // Velocidad del rayo
    float grosor;  // Grosor del rayo (qué tan gruesa será la línea que lo representará)
    Natural danho;  // Daño que causa el rayo al impactar al personaje
    bool activo;  // Si el rayo está activo o no (si se ha disparado)
    float porcentaje_progreso;  // Porcentaje de progreso del rayo en su ciclo de activación/desactivación
    EtapaRayo etapa;  // Etapa actual del rayo (en espera, aparición, activo al 100%, desaparición)
    float tiempo_en_etapa;  // Tiempo que lleva el rayo en una etapa determinada (en espera, aparición, activo al 100%, desaparición)
    EfectoSonido* efecto_sonido;  // Efecto de sonido del rayo
    bool efecto_sonido_ya_empezado;
    bool efecto_sonido_ya_detenido;
    
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
    Rayo rayos[NRO_NIVELES][MAX_RAYOS];  // Rayos por nivel
    EfectoSonido* sonido_rayo;  // Efecto de sonido del rayo
    /* 
    Lista bitmaps;  // Bitmaps en una lista enlazada (para dinamismo) -> Se implementara la idea mas adelante
    */
};

#endif // STRUCTS_H