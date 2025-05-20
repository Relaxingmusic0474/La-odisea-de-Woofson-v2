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

    personaje->posicion.x = ANCHO_VENTANA * 0.1;
    personaje->posicion.y = /* ALTO_VENTANA * 0.5 */ ALTURA_PISO;  // Se coloca en el piso
    personaje->velocidad = VELOCIDAD_PERSONAJE;
    personaje->impulso = 250;  // Este es el impulso que se le da al personaje al saltar
    personaje->ancho = al_get_bitmap_width(personaje->imagen);
    personaje->alto = al_get_bitmap_height(personaje->imagen);
    personaje->bandera_dibujo = 0;  // 0: normal, ALLEGRO_FLIP_HORIZONTAL: espejo
    personaje->en_salto = false;
    personaje->tiempo_salto = 0;  // Tiempo que lleva en el salto
    personaje->inicializado = true;

    return true;
}

void mover_personaje(Personaje* personaje, bool teclas[ALLEGRO_KEY_MAX])
{
    if (teclas[ALLEGRO_KEY_UP] && personaje->en_salto == false)
    {
        personaje->en_salto = true;
        // personaje->tiempo_salto = 0;
        //  personaje->posicion.y -= personaje->velocidad;
        // continuar_salto(personaje, 0);
        // personaje->en_salto = true;
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

void continuar_salto(Personaje* personaje, double t)
{
    // El salto es un lanzamiento vertical, así que se ocupará la física de un proyectil
    // a = dv/dt; v = dy/dt; y = y0 + v0*t + 1/2*a*t^2
    // Pero a = -g, así que y = y0 + v0*t - 1/2*g*t^2
    // En las ventanas de Allegro, el eje y crece hacia abajo, por lo que en realidad y = y0 - v0*t + g/2*t^2
    // En este caso, y0 = ALTURA_PISO, v0 = impulso, g = 400 px/s^2 (gravedad para el juego)

    personaje->posicion.y = ALTURA_PISO - personaje->impulso * t + g * t * t / 2;

    // Si el personaje llega al piso, se detiene el salto
    if (personaje->posicion.y >= ALTURA_PISO)
    {
        personaje->posicion.y = ALTURA_PISO;
        personaje->en_salto = false;
        personaje->tiempo_salto = 0;
    }

    return;
}

bool es_tecla_lateral(Tecla tecla)
{
    return (tecla == ALLEGRO_KEY_LEFT || tecla == ALLEGRO_KEY_RIGHT);
}