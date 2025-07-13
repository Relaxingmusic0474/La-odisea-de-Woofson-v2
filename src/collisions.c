#include "collisions.h"

/**
 * Función que verifica si el personaje está colisionando en alguna dirección.
 * @param personaje El personaje a verificar.
 * @param mapa El mapa del juego, que contiene los bloques y obstáculos.
 * @return true si detecta alguna colisión, false en caso contrario.
 */
bool hay_colision_con_bordes(Personaje* personaje, Mapa mapa)
{
    if (hay_colision_izquierda(*personaje, mapa) || hay_colision_derecha(*personaje, mapa) ||
        hay_colision_superior(personaje, mapa) || hay_colision_inferior(personaje, mapa))        
    {
        return true;
    }

    return false;
}


bool hay_colision_izquierda(Personaje personaje, Mapa mapa)
{
    if (personaje.posicion.x < 0)
    {
        return true;
    }

    else
    {
        return false;
    }
}


bool hay_colision_derecha(Personaje personaje, Mapa mapa)
{
    if (personaje.posicion.x + personaje.ancho - 1 > ANCHO_VENTANA)
    {
        return true;
    }

    return false;
}


bool hay_colision_superior(Personaje* personaje, Mapa mapa)
{
    if (personaje->posicion.y < 0)
    {
        personaje->salto.altura_choque = 0;
        personaje->salto.es_interrumpido = true;
        return true;
    }
    
    return hay_bloque_arriba(personaje, mapa);
}


bool hay_colision_inferior(Personaje* personaje, Mapa mapa)
{
    if (personaje->posicion.y + personaje->alto >= ALTURA_PISO)
    {
        personaje->posicion.y = ALTURA_PISO - personaje->alto;
        personaje->salto.en_salto = false;
        personaje->en_plataforma = false;
        return true;
    }

    return hay_bloque_debajo(personaje, mapa);
}


/**
 * Función que verifica si hay un bloque debajo del personaje.
 * @param personaje El personaje a verificar.
 * @param altura_bloque La altura del bloque debajo del personaje.
 * @return true si hay un bloque debajo del personaje, false en caso contrario.
 */
bool hay_bloque_debajo(Personaje* personaje, Mapa mapa)
{
    int fil, col;
    Natural i, nro_pasos, verificaciones;
    float paso, x0, x, y_inf;

    x0 = personaje->posicion.x;
    y_inf = personaje->posicion.y + personaje->alto;

    fil = (int) (y_inf / mapa.alto_bloque);

    nro_pasos = (Natural) ceilf(personaje->ancho / mapa.ancho_bloque);
    verificaciones = nro_pasos + 1;  // Considerando que los puntos a evaluar son 1 más que los subintervalos
    paso = personaje->ancho / nro_pasos;  // Ancho de cada paso de verificación

    for (i=0; i<verificaciones; i++)
    {
        x = x0 + i * paso;

        col = (int) (x / mapa.ancho_bloque);

        if (col >= 0 && col < mapa.nro_columnas)
        {
            if (mapa.mapa[fil][col] == 1)
            {
                personaje->en_plataforma = true;
                personaje->salto.altura_choque =  fil * mapa.alto_bloque - personaje->alto;
                return true;  // Hay un bloque debajo del personaje
            }
        }
    }

    return false;  // No hay bloque debajo del personaje
}


bool hay_bloque_arriba(Personaje* personaje, Mapa mapa)
{
    int fil, col;
    Natural i, nro_pasos, verificaciones;
    float paso, x0, x, y_sup;

    x0 = personaje->posicion.x;
    y_sup = personaje->posicion.y;

    fil = (int) (y_sup / mapa.alto_bloque);

    nro_pasos = (Natural) ceilf(personaje->ancho / mapa.ancho_bloque);
    verificaciones = nro_pasos + 1;  // Considerando los extremos izquierdo y derecho del personaje
    paso = personaje->ancho / nro_pasos;  // Ancho de cada paso de verificación

    for (i=0; i<verificaciones; i++)
    {
        x = x0 + i * paso;

        col = (int) (x / mapa.ancho_bloque);

        if (col >= 0 && col < mapa.nro_columnas)
        {
            if (mapa.mapa[fil][col] == 1)
            {
                personaje->salto.altura_choque = (fil+1) * mapa.alto_bloque;
                personaje->salto.es_interrumpido = true;
                return true;
            }
        }
    }

    return false;
}


bool hay_bloque_izquierda(Personaje* personaje, Mapa mapa)
{
    int fil, col;
    float x_izq, y_inf;
    
    x_izq = personaje->posicion.x;
    y_inf = personaje->posicion.y + personaje->alto;

    fil = (int) (y_inf / mapa.alto_bloque);
    col = (int) (x_izq / mapa.ancho_bloque);

    if (col >= 0 && col < mapa.nro_columnas)
    {
        if (mapa.mapa[fil][col] == 1)
        {

            return true;
        }
    }

    return false;
}


Procedure efectuar_colision(Personaje* personaje, Mapa mapa)
{
    if (hay_colision_izquierda(*personaje, mapa))
    {
        personaje->posicion.x = 0;
    }

    else if (hay_colision_derecha(*personaje, mapa))
    {
        personaje->posicion.x = ANCHO_VENTANA - personaje->ancho;
    }

    else if (hay_colision_superior(personaje, mapa))
    {
        personaje->posicion.y = personaje->salto.altura_choque;
        personaje->salto.tiempo_en_salto = 0;  /* Reinicia el tiempo de salto para evitar problemas de acumulación */
        personaje->salto.altura_inicial = personaje->salto.altura_choque;
        personaje->salto.impulso = personaje->velocidad.y;  /* Reinicia el impulso del salto */
        personaje->velocidad.y = -personaje->velocidad.y;  /* Invierte la velocidad en el eje y */
    }

    else  
    {
        if (!personaje->en_plataforma)  /* Para el suelo */
        {
            personaje->salto.altura_choque = personaje->posicion.y;  /* Guarda la altura del choque con el suelo o bloque */
            personaje->posicion.y = ALTURA_PISO - personaje->alto;  /* Ajusta la posición del personaje al suelo */
            personaje->salto.en_salto = false;
        }

        else 
        {
            personaje->posicion.y = personaje->salto.altura_choque;  /* Ajusta la posición del personaje al suelo o bloque */
        }
        
        personaje->salto.en_salto = false;
        personaje->salto.tiempo_en_salto = 0;
        personaje->salto.altura_inicial = personaje->posicion.y;  // Reinicia la altura inicial del salto
        personaje->salto.es_interrumpido = false;  // Reinicia la variable de interrupción del salto
        personaje->salto.impulso = IMPULSO_PERSONAJE;  // Reinicia el impulso del salto
        personaje->velocidad.y = 0;  // Detiene la velocidad en el eje y
    }

    return;
}