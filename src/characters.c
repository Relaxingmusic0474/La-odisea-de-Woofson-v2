#include "characters.h"

bool inicializar_personaje(Personaje* personaje, char tipo)
{
    switch (tipo)
    {
        case 'W':
            personaje->tipo = 'W';  // Woofson
            personaje->velocidad = VELOCIDAD_PERSONAJE;
            personaje->imagen = al_load_bitmap("assets/images/woofson.png");
            break;

        case 'D':
            personaje->tipo = 'D';  // Dragón
            personaje->velocidad = VELOCIDAD_DRAGONES;
            personaje->imagen = al_load_bitmap("assets/images/dragon.png");
            break;

        case 'H':
            personaje->tipo = 'H';  // Humanoide
            personaje->velocidad = VELOCIDAD_HUMANOIDES;
            personaje->imagen = al_load_bitmap("assets/images/humanoide.png");
            break;

        case 'J':
            personaje->tipo = 'J';  // Jefe
            personaje->velocidad = VELOCIDAD_JEFE;
            personaje->imagen = al_load_bitmap("assets/images/jefe.png");
            break;
    }

    if (!personaje->imagen)
    {
        printf("Error al cargar la imagen del personaje.\n");
        return false;
    }

    personaje->posicion.x = SCREEN_WIDTH * 0.1;
    personaje->posicion.y = SCREEN_HEIGHT * 0.5;
    // personaje->velocidad = VELOCIDAD_PERSONAJE;
    personaje->ancho = al_get_bitmap_width(personaje->imagen);
    personaje->alto = al_get_bitmap_height(personaje->imagen);
    personaje->bandera_dibujo = 0;  // 0: normal, ALLEGRO_FLIP_HORIZONTAL: espejo
    personaje->inicializado = true;

    return true;
}

void mover_personaje(Personaje* personaje, bool teclas[ALLEGRO_KEY_MAX])
{
    if (teclas[ALLEGRO_KEY_UP])
    {
        personaje->posicion.y -= personaje->velocidad;
    }

    if (teclas[ALLEGRO_KEY_DOWN])
    {
        personaje->posicion.y += personaje->velocidad;
    }

    if (teclas[ALLEGRO_KEY_LEFT])
    {
        personaje->posicion.x -= personaje->velocidad;
    }

    if (teclas[ALLEGRO_KEY_RIGHT])
    {
        personaje->posicion.x += personaje->velocidad;
    }

    return;
}

bool es_tecla_lateral(Tecla tecla)
{
    return (tecla == ALLEGRO_KEY_LEFT || tecla == ALLEGRO_KEY_RIGHT);
}