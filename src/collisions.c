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
        hay_colision_superior(*personaje, mapa) || hay_colision_inferior(personaje, mapa))        
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


bool hay_colision_superior(Personaje personaje, Mapa mapa)
{
    if (personaje.posicion.y < 0 || hay_bloque_arriba(&personaje, mapa))
    {
        return true;
    }

    else
    {
        return false;
    }
}

bool hay_colision_inferior(Personaje* personaje, Mapa mapa)
{
    if (personaje->posicion.y + personaje->alto >= ALTURA_PISO || hay_bloque_debajo(personaje, mapa))
    {
        
        return true;
    }

    else
    {
        return false;
    }
}



/**
 * Función que verifica si hay un bloque debajo del personaje.
 * @param personaje El personaje a verificar.
 * @param altura_bloque La altura del bloque debajo del personaje.
 * @return true si hay un bloque debajo del personaje, false en caso contrario.
 */
bool hay_bloque_debajo(Personaje* personaje, Mapa mapa)
{
    Natural i, j;
    Natural ancho_bloque = ANCHO_VENTANA / mapa.nro_columnas;  /* Ancho de cada bloque en el mapa */
    Natural alto_bloque = ALTURA_PISO / mapa.nro_filas;  /* Alto de cada bloque en el mapa */
    Vector posicion_bloque;
    bool col_x, col_y;

    for (i=0; i<mapa.nro_filas; i++)
    {
        for (j=0; j<mapa.nro_columnas; j++)
        {
            if (mapa.mapa[i][j] == 1)   /* Si hay un bloque en la posición (i, j) */
            {
                posicion_bloque.x = j * ancho_bloque;   /* Posición x del bloque */
                posicion_bloque.y = i * alto_bloque;   /* Posición y del bloque */

                col_x = personaje->posicion.x + personaje->ancho / 2 > posicion_bloque.x && personaje->posicion.x + personaje->ancho / 2 < posicion_bloque.x + ancho_bloque;
                col_y = personaje->posicion.y + personaje->alto >= posicion_bloque.y && personaje->posicion.y + personaje->alto <= posicion_bloque.y + alto_bloque;

                if (col_x && col_y)
                {
                    personaje->salto.en_salto = false;
                    // personaje->salto.altura_choque = posicion_bloque.y;  /* Guarda la altura del choque con el bloque */
                    personaje->en_plataforma = true;  /* Indica que el personaje está en una plataforma */
                    
                    return true;
                }
            }
        }
    }

    return false;
}



/* ADVERTENCIA: PUEDE NO ESTAR BIEN ESTA FUNCION */
bool hay_bloque_arriba(Personaje* personaje, Mapa mapa)
{
    return false;  /* Por ahora no se implementa la colisión con bloques arriba del personaje */
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

    else if (hay_colision_superior(*personaje, mapa))
    {
        personaje->posicion.y = 0;
    }

    else  /* Para el suelo */
    {
        personaje->salto.altura_choque = personaje->posicion.y;  /* Guarda la altura del choque con el suelo o bloque */

        if (!personaje->en_plataforma)
        {
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