#include "collisions.h"

/**
 * Función que verifica si el personaje está colisionando en alguna dirección.
 * @param personaje El personaje a verificar.
 * @param mapa El mapa del juego, que contiene los bloques y obstáculos.
 * @return true si detecta alguna colisión, false en caso contrario.
 */
bool hay_colision_con_bordes(Personaje* personaje, Mapa mapa)
{
    if (hay_colision_izquierda(personaje, mapa) || hay_colision_derecha(personaje, mapa) ||
        hay_colision_superior(personaje, mapa) || hay_colision_inferior(personaje, mapa))        
    {
        return true;
    }

    return false;
}


/**
 * Función que verifica si hay una colisión superior con el personaje.
 * @param personaje El personaje a verificar.
 * @param mapa El mapa del juego, que contiene los bloques y obstáculos.
 * @return true si hay una colisión superior, false en caso contrario.
 */
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


/**
 * Función que verifica si hay un bloque arriba del personaje.
 * @param personaje El personaje a verificar.
 * @param mapa El mapa del juego, que contiene los bloques y obstáculos.
 * @return true si hay un bloque arriba del personaje, false en caso contrario.
 */
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


/**
 * Función que verifica si hay una colisión inferior con el personaje.
 * @param personaje El personaje a verificar.
 * @param mapa El mapa del juego, que contiene los bloques y obstáculos.
 * @return true si hay una colisión inferior, false en caso contrario.
 */
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


/**
 * Función que verifica si hay una colisión a la izquierda del personaje.
 * @param personaje El personaje a verificar.
 * @param mapa El mapa del juego, que contiene los bloques y obstáculos.
 * @return true si hay una colisión a la izquierda, false en caso contrario.
 */
bool hay_colision_izquierda(Personaje* personaje, Mapa mapa)
{
    if (personaje->posicion.x < 0)
    {
        personaje->posicion.x = 0;  // Ajusta la posición del personaje al borde izquierdo de la ventana
        return true;
    }

    return hay_bloque_izquierda(personaje, mapa);
}


/**
 * Función que verifica si hay un bloque a la izquierda del personaje.
 * @param personaje El personaje a verificar.
 * @param mapa El mapa del juego, que contiene los bloques y obstáculos.
 * @return true si hay un bloque a la izquierda, false en caso contrario.
 */
bool hay_bloque_izquierda(Personaje* personaje, Mapa mapa)
{
    int fil, col;
    Natural i, nro_pasos, verificaciones;
    float paso, x_izq, y0, y;

    x_izq = personaje->posicion.x + 1;
    y0 = personaje->posicion.y + 1;

    col = (int) (x_izq / mapa.ancho_bloque);

    nro_pasos = (Natural) ceilf(personaje->alto / mapa.alto_bloque);
    verificaciones = nro_pasos + 1;  // Considerando los extremos superior e inferior del personaje
    paso = (personaje->alto-2) / nro_pasos;  // Alto de cada paso de verificación

    for (i=0; i<verificaciones; i++)
    {
        y = y0 + i * paso;

        fil = (int) (y / mapa.alto_bloque);

        if (fil >= 0 && fil < mapa.nro_filas)
        {
            if (mapa.mapa[fil][col] == 1)
            {
                personaje->posicion.x = (col + 1) * mapa.ancho_bloque;  // Ajusta la posición del personaje al borde izquierdo del bloque
                return true;
            }
        }
    }

    return false;
}


/**
 * Función que verifica si hay una colisión a la derecha del personaje.
 * @param personaje El personaje a verificar.
 * @param mapa El mapa del juego, que contiene los bloques y obstáculos.
 * @return true si hay una colisión a la derecha, false en caso contrario.
 */
bool hay_colision_derecha(Personaje* personaje, Mapa mapa)
{
    if (personaje->posicion.x + personaje->ancho - 1 > ANCHO_VENTANA)
    {
        personaje->posicion.x = ANCHO_VENTANA - personaje->ancho;  // Ajusta la posición del personaje al borde derecho de la ventana
        return true;
    }

    return hay_bloque_derecha(personaje, mapa);
}


/**
 * Función que verifica si hay un bloque a la derecha del personaje.
 * @param personaje El personaje a verificar.
 * @param mapa El mapa del juego, que contiene los bloques y obstáculos.
 * @return true si hay un bloque a la derecha, false en caso contrario.
 */
bool hay_bloque_derecha(Personaje* personaje, Mapa mapa)
{
    int fil, col;
    Natural i, nro_pasos, verificaciones;
    float paso, x_der, y0, y;

    x_der = personaje->posicion.x + personaje->ancho;
    y0 = personaje->posicion.y + 1;

    col = (int) (x_der / mapa.ancho_bloque);

    nro_pasos = (Natural) ceilf(personaje->alto / mapa.alto_bloque);
    verificaciones = nro_pasos + 1;  // Considerando los extremos superior e inferior del personaje
    paso = (personaje->alto-2) / nro_pasos;  // Alto de cada paso de verificación

    for (i=0; i<verificaciones; i++)
    {
        y = y0 + i * paso;

        fil = (int) (y / mapa.alto_bloque);

        if (fil >= 0 && fil < mapa.nro_filas)
        {
            if (mapa.mapa[fil][col] == 1)
            {
                personaje->posicion.x = col * mapa.ancho_bloque - personaje->ancho;  // Ajusta la posición del personaje al borde derecho del bloque
                return true;
            }
        }
    }

    return false;
}


/**
 * Función que efectúa la colisión del personaje con el mapa, ajustando su posición y estado según la colisión detectada.
 * @param personaje El personaje que está colisionando.
 * @param mapa El mapa del juego, que contiene los bloques y obstáculos.
 */
Procedure efectuar_colision(Personaje* personaje, Mapa mapa)
{
    extern bool teclas[ALLEGRO_KEY_MAX];  // Arreglo global de teclas presionadas

    if (hay_colision_superior(personaje, mapa))
    {
        personaje->posicion.y = personaje->salto.altura_choque;
        personaje->salto.tiempo_en_salto = 0;  /* Reinicia el tiempo de salto para evitar problemas de acumulación */
        personaje->salto.altura_inicial = personaje->salto.altura_choque;
        personaje->salto.impulso = personaje->velocidad.y;  /* Reinicia el impulso del salto */
        personaje->velocidad.y = -personaje->velocidad.y;  /* Invierte la velocidad en el eje y */
    }

    if (hay_colision_inferior(personaje, mapa))  
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
    
    if (hay_colision_izquierda(personaje, mapa) && !teclas[ALLEGRO_KEY_RIGHT])
    {
        patalear(personaje, -1);  /* Patalear hacia la izquierda */
    }   
    
    if (hay_colision_derecha(personaje, mapa) && !teclas[ALLEGRO_KEY_LEFT])
    {
        patalear(personaje, 1);  /* Patalear hacia la derecha */
    }

    return;
}