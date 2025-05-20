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

    personaje->ancho = al_get_bitmap_width(personaje->imagen);
    personaje->alto = al_get_bitmap_height(personaje->imagen);
    personaje->posicion.x = ANCHO_VENTANA * 0.1;
    personaje->posicion.y = /* ALTO_VENTANA * 0.5 */ ALTURA_PISO - personaje->alto;  // Se coloca en el piso
    personaje->velocidad = VELOCIDAD_PERSONAJE;
    personaje->impulso = 250;  // Este es el impulso que se le da al personaje al saltar
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

    if (teclas[ALLEGRO_KEY_DOWN] && !hay_colision_inferior(*personaje))
    {
        personaje->posicion.y += personaje->velocidad;
    }

    if (teclas[ALLEGRO_KEY_LEFT] && !hay_colision_izquierda(*personaje))
    {
        personaje->posicion.x -= personaje->velocidad;
    }

    if (teclas[ALLEGRO_KEY_RIGHT] && !hay_colision_derecha(*personaje))
    {
        personaje->posicion.x += personaje->velocidad;
    }
    
    if (hay_colision_con_bordes(*personaje))
    {
        efectuar_colision_con_bordes(personaje);
    }

    return;
}

bool hay_colision_con_bordes(Personaje personaje)
{
    if (hay_colision_izquierda(personaje) || hay_colision_derecha(personaje) ||
        hay_colision_superior(personaje) || hay_colision_inferior(personaje))        
    {
        return true;
    }

    return false;
}

bool hay_colision_izquierda(Personaje personaje)
{
    if (personaje.posicion.x <= 0)
    {
        return true;
    }

    else
    {
        return false;
    }
}

bool hay_colision_derecha(Personaje personaje)
{
    if (personaje.posicion.x + personaje.ancho >= ANCHO_VENTANA)
    {
        return true;
    }

    return false;
}

bool hay_colision_superior(Personaje personaje)
{
    if (personaje.posicion.y <= 0)
    {
        return true;
    }

    else
    {
        return false;
    }
}

bool hay_colision_inferior(Personaje personaje)
{
    if (personaje.posicion.y + personaje.alto >= ALTURA_PISO)
    {
        return true;
    }

    else
    {
        return false;
    }
}

void efectuar_colision_con_bordes(Personaje* personaje)
{
    if (hay_colision_izquierda(*personaje))
    {
        personaje->posicion.x == 0;
    }

    else if (hay_colision_derecha(*personaje))
    {
        personaje->posicion.x == ANCHO_VENTANA - personaje->ancho;
    }

    else if (hay_colision_superior(*personaje))
    {
        personaje->posicion.y == 0;
    }

    else
    {
        personaje->posicion.y == ALTURA_PISO - personaje->alto;
    }

    return;
}

void continuar_salto(Personaje* personaje, double t)
{
    // El salto es un lanzamiento vertical, así que se ocupará la física de un proyectil
    // a = dv/dt; v = dy/dt; y = y0 + v0*t + 1/2*a*t^2
    // Pero a = -g, así que y = y0 + v0*t - 1/2*g*t^2
    // En las ventanas de Allegro, el eje y crece hacia abajo, por lo que en realidad y = y0 - v0*t + g/2*t^2
    // En este caso, y0 = posicion.y, v0 = impulso, g = 400 px/s^2 (gravedad para el juego)

    personaje->posicion.y = (ALTURA_PISO - personaje->alto) - personaje->impulso * t + g * t * t / 2;

    // Si el personaje llega al piso, se detiene el salto
    if (hay_colision_inferior(*personaje) /* || hay_colision_con_bloque() || hay_colision_con_objeto() */)
    {
        personaje->posicion.y = ALTURA_PISO - personaje->alto;
        personaje->en_salto = false;
        personaje->tiempo_salto = 0;
    }

    return;
}

bool es_tecla_lateral(Tecla tecla)
{
    return (tecla == ALLEGRO_KEY_LEFT || tecla == ALLEGRO_KEY_RIGHT);
}