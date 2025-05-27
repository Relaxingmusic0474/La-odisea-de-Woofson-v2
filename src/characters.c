#include "characters.h"

bool inicializar_personaje(Personaje* personaje, char tipo)
{
    switch (tipo)
    {
        case 'W':
            personaje->tipo = 'W';  // Woofson
            personaje->velocidad.x = VELOCIDAD_PERSONAJE;
            personaje->velocidad.y = 0;  // Inicialmente no está en salto
            personaje->imagen = al_load_bitmap("assets/images/woofson.png");
            break;

        case 'D':
            personaje->tipo = 'D';  // Dragón
            personaje->velocidad.x = VELOCIDAD_DRAGONES;
            personaje->velocidad.y = 0;  // Inicialmente no está en salto
            personaje->imagen = al_load_bitmap("assets/images/dragon.png");
            break;

        case 'H':
            personaje->tipo = 'H';  // Humanoide
            personaje->velocidad.x = VELOCIDAD_HUMANOIDES;
            personaje->velocidad.y = 0;  // Inicialmente no está en salto
            personaje->imagen = al_load_bitmap("assets/images/humanoide.png");
            break;

        case 'J':
            personaje->tipo = 'J';  // Jefe
            personaje->velocidad.x = VELOCIDAD_JEFE;
            personaje->velocidad.y = 0;  // Inicialmente no está en salto
            personaje->imagen = al_load_bitmap("assets/images/jefe.png");
            break;
    }

    if (!personaje->imagen)
    {
        printf("Error al cargar la imagen del personaje.\n");
        return false;
    }

    personaje->escala_dibujo = 1.0;  // Escala de dibujo del personaje (para que se vea más grande o más pequeño)
    personaje->ancho = al_get_bitmap_width(personaje->imagen) * personaje->escala_dibujo;  // Ancho del personaje
    personaje->alto = al_get_bitmap_height(personaje->imagen) * personaje->escala_dibujo;  // Alto del personaje
    personaje->posicion.x = ANCHO_VENTANA * 0.1;
    personaje->posicion.y = /* ALTO_VENTANA * 0.5 */ ALTURA_PISO - personaje->alto;  // Se coloca en el piso
    // personaje->impulso = 800;  // Este es el impulso que se le da al personaje al saltar
    personaje->bandera_dibujo = 0;  // 0: normal, ALLEGRO_FLIP_HORIZONTAL: espejo
    inicializar_salto(personaje);  // Inicializa la estructura del salto para el personaje
    // personaje->inicializado = true;

    return true;
}


Procedure inicializar_salto(Personaje* personaje)
{
    personaje->salto.en_salto = false;
    personaje->salto.es_interrumpido = false;  // Indica si el salto es interrumpido por una colisión con algún techo o no
    personaje->salto.tiempo_en_salto = 0;  // Reinicia el tiempo de salto
    personaje->salto.impulso = IMPULSO_PERSONAJE;  // Si bien no es necesario, es una buena práctica para la legibilidad
    personaje->salto.altura_inicial = personaje->posicion.y;  // Reinicia la altura inicial del salto
    // personaje->en_salto = false;
    // personaje->tiempo_salto = 0;
    return;
}


Procedure mover_personaje(Personaje* personaje, bool teclas[ALLEGRO_KEY_MAX])
{
    // extern bool teclas[ALLEGRO_KEY_MAX];  // Declaración de las teclas presionadas (variable extern)

    if (teclas[ALLEGRO_KEY_UP] && personaje->salto.en_salto == false && !hay_colision_superior(*personaje))
    {
        personaje->salto.en_salto = true;
        personaje->salto.altura_inicial = personaje->posicion.y;  // Guarda la altura inicial del salto
        personaje->salto.tiempo_en_salto = 0;  // Reinicia el tiempo de salto
        // personaje->tiempo_salto = 0;
        //  personaje->posicion.y -= personaje->velocidad;
        // continuar_salto(personaje, 0);
        // personaje->en_salto = true;
    }

    if (teclas[ALLEGRO_KEY_DOWN] && !hay_colision_inferior(*personaje))
    {
        personaje->posicion.y += personaje->velocidad.y;
    }

    if (teclas[ALLEGRO_KEY_LEFT] && !hay_colision_izquierda(*personaje))
    {
        personaje->posicion.x -= personaje->velocidad.x;
    }

    if (teclas[ALLEGRO_KEY_RIGHT] && !hay_colision_derecha(*personaje))
    {
        personaje->posicion.x += personaje->velocidad.x;
    }
    
    if (hay_colision_con_bordes(*personaje))
    {
        efectuar_colision_con_bordes(personaje);
    }

    return;
}

/**
 * Función que verifica si el personaje está colisionando en alguna dirección.
 * @param personaje El personaje a verificar.
 * @return true si detecta alguna colisión, false en caso contrario.
 */
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
    if (personaje.posicion.y < 0)
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

Procedure efectuar_colision_con_bordes(Personaje* personaje)
{
    if (hay_colision_izquierda(*personaje))
    {
        personaje->posicion.x = 0;
    }

    else if (hay_colision_derecha(*personaje))
    {
        personaje->posicion.x = ANCHO_VENTANA - personaje->ancho;
    }

    else if (hay_colision_superior(*personaje))
    {
        personaje->posicion.y = 0;
    }

    else  // Para la colisión inferior
    {
        personaje->posicion.y = ALTURA_PISO - personaje->alto;
    }

    return;
}


Procedure continuar_salto(Personaje* personaje, float t)
{
    /* 
        El salto es un lanzamiento vertical, así que se ocupará la física de un proyectil
        a = dv/dt; v = dy/dt; y = y0 + v0*t + 1/2*a*t^2
        Pero a = -g, así que y = y0 + v0*t - 1/2*g*t^2
        En las ventanas de Allegro, el eje y crece hacia abajo, por lo que en realidad y = y0 - v0*t + g/2*t^2
        En este caso, y0 = posicion.y, v0 = impulso, g = 550 px/s^2 (gravedad para el juego)
    */

    personaje->posicion.y = personaje->salto.altura_inicial - personaje->salto.impulso * t + g * t * t / 2;
    personaje->velocidad.y = velocidad_instantanea(*personaje, t);  // Actualiza la velocidad en el eje y

    if (hay_colision_superior(*personaje))
    {
        personaje->salto.es_interrumpido = true;  // Marca que el salto es interrumpido por la colisión con el techo
        personaje->posicion.y = 0;
        personaje->salto.tiempo_en_salto = 0;  // Reinicia el tiempo de salto para evitar problemas de acumulación
        personaje->salto.altura_choque = personaje->posicion.y;  // Guarda la altura del choque con el techo
        personaje->salto.altura_inicial = personaje->salto.altura_choque;
        personaje->salto.impulso = personaje->velocidad.y;  // Reinicia el impulso del salto
        personaje->velocidad.y = -personaje->velocidad.y;  // Invierte la velocidad en el eje y
        
    }

    // Si el personaje llega al piso, se detiene el salto
    if (hay_colision_inferior(*personaje) /* || hay_colision_con_bloque() || hay_colision_con_objeto() */)
    {
        personaje->posicion.y = ALTURA_PISO - personaje->alto;
        personaje->salto.en_salto = false;
        personaje->salto.tiempo_en_salto = 0;
        personaje->salto.altura_inicial = personaje->posicion.y;  // Reinicia la altura inicial del salto
        personaje->salto.es_interrumpido = false;  // Reinicia la variable de interrupción del salto
        personaje->salto.impulso = IMPULSO_PERSONAJE;  // Reinicia el impulso del salto
        personaje->velocidad.y = 0;  // Detiene la velocidad en el eje y
        
    }

    return;
}


Procedure rebotar_con_techo(Personaje* personaje, float t)
{
    // Si el personaje colisiona con el techo, se invierte la velocidad en el eje y
    personaje->velocidad.y = -personaje->velocidad.y;  // Invierte la velocidad en el eje y
    personaje->posicion.y = personaje->salto.altura_inicial;  // Ajusta la posición para evitar que se salga por arriba 
    // mover_personaje(personaje, teclas);  // Detiene el movimiento lateral

    return;
}


Procedure colisionar_con_techo(Personaje* personaje, Entero altura_techo)
{
    // Si el personaje colisiona con el techo, se invierte la velocidad en el eje y
    personaje->velocidad.y = -personaje->velocidad.y;  // Invierte la velocidad en el eje y
    personaje->posicion.y = altura_techo;  // Ajusta la posición para evitar que se salga por arriba 
    // mover_personaje(personaje, teclas);  // Detiene el movimiento lateral

    return;
}


Procedure caida_libre(Personaje* personaje, float t)
{
    // La caída libre es un movimiento uniformemente acelerado, así que se ocupará la física de un proyectil
    // a = dv/dt; v = dy/dt; y = y0 + v0*t + 1/2*a*t^2
    // Pero a = g, así que y = y0 + v0*t + 1/2*g*t^2
    // En este caso, y0 = posicion.y, v0 = 0 (porque no hay impulso inicial), g = 550 px/s^2 (gravedad para el juego)

    personaje->posicion.y += g * t * t / 2;  // Actualiza la posición en el eje y
    personaje->velocidad.y += g * t;  // Actualiza la velocidad en el eje y

    return;
}


Entero velocidad_instantanea(Personaje personaje, float t)
{
    // La velocidad instantánea en el salto es v = v0 - g*t
    // En este caso, v0 = impulso, g = 550 px/s^2 (gravedad para el juego)
    return -personaje.salto.impulso + g*t;
}

/**
 * Función que verifica si una tecla es una tecla lateral (izquierda o derecha).
 * @param tecla La tecla a verificar.
 * @return true si la tecla es una tecla lateral, false en caso contrario.
 */
bool es_tecla_lateral(Tecla tecla)  // El tipo Tecla es un int, pero se usa Tecla para mayor legibilidad
{
    return (tecla == ALLEGRO_KEY_LEFT || tecla == ALLEGRO_KEY_RIGHT);
}