#ifndef STRUCTS_H
#define STRUCTS_H

#include "allegro_libraries.h"
#include "macros.h"
#include "types.h"
#include "enums.h"

typedef struct vector Vector;
typedef struct rectangulo Rectangulo;
typedef struct barraVida BarraVida;
typedef struct salto Salto;
typedef struct mapa Mapa;
typedef struct bala Bala;
typedef union formaAtaque FormaAtaque;
typedef struct arma Arma;
typedef struct personaje Personaje;
typedef struct colision Colision;
typedef struct boton Boton; 
typedef struct menu Menu; 
typedef struct musica Musica;
typedef struct efectoSonido EfectoSonido;
typedef struct rayo Rayo;
typedef struct regresionLineal RegresionLineal;
typedef struct espina Espina;
typedef struct palanca Palanca;
typedef struct puerta Puerta;
typedef struct pocion Pocion;
typedef struct municion Municion;
typedef struct fuego Fuego;
typedef struct datos Datos;
typedef struct ranking Ranking;
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

struct barraVida
{
    Rectangulo contenedor;
    Rectangulo subvida;
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

struct bala
{
    Vector posicion_inicial;
    Vector posicion;
    Vector velocidad;
    Entero direccion;  // 1 si es hacia la derecha y -1 si es hacia la izquierda
    bool activa;
    bool disponible;
    float frames_para_disponibilidad;  // Este campo solo cobra sentido si es que las balas son recargables con el tiempo (personaje->bala_recargable == true)
};

union formaAtaque
{
    Bala balas[10];
    // Otra para cuerpo a cuerpo
};

struct arma
{
    FormaAtaque ataque;
};

struct fuego
{
    Imagen imagen;
    Vector posicion;
    float porcentaje_progreso;
    float alto;
    float ancho;
};

struct personaje
{
    TipoPersonaje tipo;
    Natural cantidad_frames;
    Imagen* frames;  // Arreglo de imágenes para las animaciones del personaje
    Imagen imagen;  // Imagen actual del personaje
    Natural id_nro_frame; // Número del frame actual del personaje (para las animaciones)
    Natural nro_vidas;
    Natural subvida_actual;
    Vector posicion;
    Vector posicion_inicial;
    Vector velocidad;
    float ancho;
    float alto;
    int bandera_dibujo;  // Si se dibuja normal o en espejo
    int direccion;  // Dirección de movimiento (1: derecha; -1: izquierda)
    bool en_plataforma;  // Si el personaje está en una plataforma o no (si está en el suelo es false)
    bool caminata;  // Si el personaje está en movimiento o intentando moverse (caminando, corriendo o pataleando)
    unsigned int fps_en_caminata;  // Número de frames en caminata (para la animación)
    bool en_ataque;
    unsigned int fps_en_ataque; // Número de frames en ataque (para la animación)
    Salto salto;
    bool danhado;
    float tiempo_danho;
    bool muerto;
    float tiempo_muerte;
    Imagen imagen_modo_muerte;
    bool estatico;
    bool inicializado;
    Bala balas[MAX_BALAS];
    Natural balas_disponibles;
    bool bala_recargable;
    Natural frames_para_prox_disparo;
    bool victoria;
    Fuego fuego;  // Aplica solo para el dragón
    BarraVida barra_vida;
    bool bala_maximo_alcance;
    float tiempo_restante_bala_maximo_alcance;
    // bool hay_obj_izq;
    // bool hay_obj_der;
    // bool hay_obj_sup;
    // CREAR UN ARREGLO DE MUNICIONES
    
};

struct boton
{
    Rectangulo rectangulo;
    char texto[LARGO]; 
}; 

struct menu 
{ 
    Rectangulo rect_destino;
    Imagen fondo; 
    ALLEGRO_FONT* fuente; 
    ALLEGRO_FONT* fuente_sec;
    Boton* opciones;
    Natural nro_opciones;
    Natural opcion_en_hover;
    TipoMenu tipo;
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

struct regresionLineal
{
    float m;  // Pendiente
    float n;  // Ordenada al origen
};

struct espina
{
    Imagen imagen_espina;
    float ancho;
    float alto;
    RegresionLineal regresiones[NRO_REGRESIONES];  // Las espinas son triangulares, así que se pueden representar mediante dos regresiones lineales
    Vector posicion_vertice;
    char direccion_espina;  // Si la espina es 'I' (inferior: está abajo y hay que saltarla), 'S' (superior), 'L' (izquierda: left) o 'R' (derecha: right)
};

struct palanca
{
    Vector posicion;
    Imagen imagen;
    EstadoPalanca estado;
    float alto;
    float ancho;
};

struct puerta
{
    Vector posicion;
    Imagen imagen;
    EstadoPuerta estado;
    float alto;
    float ancho;
};

struct pocion
{
    Imagen imagen;
    bool tomada;
};

struct municion
{
    Imagen imagen;
    bool tomada;
};



struct datos
{
    char nombre[LARGO];
    Natural puntaje;
    Natural nro_nivel;
};

struct ranking
{
    Datos datos[MAX_DATOS];
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
    Imagen vida;
    Imagen bloques[NRO_BLOQUES];
    Imagen espina;
    Rayo rayos[NRO_NIVELES][MAX_RAYOS];  // Rayos por nivel
    Natural cantidad_rayos[NRO_NIVELES];
    EfectoSonido* sonido_rayo;  // Efecto de sonido del rayo
    Espina espina_actual;  // POSIBLEMENTE QUITE LAS ESPINAS
    Imagen fondos[NRO_NIVELES];
    Imagen fondo_menu_principal;
    Imagen fondo_menu_niveles;
    Imagen* frames[TIPOS_PERSONAJES];
    Personaje enemigos[MAX_ENEMIGOS];
    Puerta puerta;
    Palanca palanca;
    Imagen puertas[NRO_ESTADOS];
    Imagen palancas[NRO_ESTADOS];
    Imagen pocion;
    Imagen pocion_rango_bala;
    Imagen municion;
    Imagen municion_pro;
    Pocion pociones[MAX_POCIONES];
    Pocion pociones_rango_bala[MAX_POCIONES_RANGO_BALA];
    Municion municiones[MAX_MUNICIONES];
    Imagen fuego;
    Ranking rankings[NRO_NIVELES];  // 1 ranking por cada nivel
    //Imagen imagen_extraterrestre;
    //Personaje extraterrestres[MAX_EXTRATERRESTRES];
    // Personaje* enemigos[NRO_NIVELES];
    
    /* 
    Lista bitmaps;  // Bitmaps en una lista enlazada (para dinamismo) -> Se implementara la idea mas adelante
    */
};

#endif // STRUCTS_H