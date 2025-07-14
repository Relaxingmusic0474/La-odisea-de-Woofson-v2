#include "traps.h"

/**
 * Función que inicializa un rayo con sus valores por defecto.
 * @param rayo Puntero al rayo a inicializar.
 * @param sonido_rayo Puntero al efecto de sonido del rayo.
 */
Procedure inicializar_rayo(Rayo* rayo, Musica* sonido_rayo)
{
    rayo->origen.x = 0;
    rayo->origen.y = 0;
    rayo->objetivo.x = 0;
    rayo->objetivo.y = 0;
    rayo->posicion.x = 0;
    rayo->posicion.y = 0;
    rayo->velocidad.x = 0;
    rayo->velocidad.y = 0;
    rayo->etapa = INOPERATIVO;
    rayo->tiempo_en_etapa = 0.0f;  // Inicializa el tiempo en etapa a 0
    rayo->porcentaje_progreso = 0.0f;  // Inicializa el porcentaje de progreso a 0
    rayo->danho = DANHO_RAYO;  // Asigna el daño del rayo
    rayo->activo = false;  // Inicialmente el rayo no está activo
    rayo->efecto_sonido = sonido_rayo;  // Se asigna el efecto de sonido del rayo
}


/**
 * Función para inicializar los rayos de todos los niveles.
 * @param rayos La matriz con los rayos de los niveles.
 * @param sonido_rayo Estructura con el efecto de sonido del rayo.
 */
Procedure inicializar_rayos(Rayo rayos[NRO_NIVELES][MAX_RAYOS], Musica* sonido_rayo)
{
    Natural i, j;

    for (i=0; i<NRO_NIVELES; i++)
    {
        for (j=0; j<MAX_RAYOS; j++)
        {
            inicializar_rayo(&rayos[i][j], sonido_rayo);
        }
    }
}


/**
 * Función que detecta los rayos que habrán en el mapa de un cierto nivel.
 * @param mapa El mapa del nivel.
 * @param rayos El arreglo con los rayos que hay en un cierto nivel.
 * @param max_rayos Máxima cantidad de rayos que podrán colocarse en un nivel.
 * @return El número de rayos que detecta en el nivel.
 */
Natural detectar_rayos(Mapa mapa, Rayo rayos[], Natural max_rayos)
{
    Natural i, j, k, t, cantidad;  // Contadores y acumuladores
    bool es_valido;  // Variable para verificar si el rayo es válido

    cantidad = 0;  // Inicializa el contador de rayos detectados

    // Recorre el mapa para detectar pares de trampas (un rayo se forma entre dos bloques trampa ubicados en la misma fila o columna)
    for (i=0; i<mapa.nro_filas; i++)
    {
        for (j=0; j<mapa.nro_columnas; j++)
        {
            if (mapa.mapa[i][j] == TRAMPA)
            {
                // Buscar en dirección horizontal
                for (k=j+1; k<mapa.nro_columnas; k++)
                {
                    if (mapa.mapa[i][k] == TRAMPA)
                    {
                        // Se verifica que no haya bloques entre las dos trampas (porque el rayo no puede atravesar bloques)
                        es_valido = true;

                        for (t=j+1; t<k; t++)
                        {
                            if (mapa.mapa[i][t] != NADA)
                            {
                                es_valido = false;
                                break;
                            }
                        }

                        if (es_valido && cantidad < max_rayos)
                        {
                            rayos[cantidad].origen.x = (j + 0.5f) * mapa.ancho_bloque;
                            rayos[cantidad].origen.y = (i + 0.5f) * mapa.alto_bloque;
                            rayos[cantidad].objetivo.x = (k + 0.5f) * mapa.ancho_bloque;
                            rayos[cantidad].objetivo.y = (i + 0.5f) * mapa.alto_bloque;
                            rayos[cantidad].velocidad.x = (rayos[cantidad].objetivo.x - rayos[cantidad].origen.x) / TIEMPO_RAYO_EN_APARICION;  // Velocidad horizontal del rayo
                            cantidad++;
                        }

                        break;  // Solo detectamos el primer par
                    }
                }

                // Buscar en dirección vertical
                for (k=i+1; k<mapa.nro_filas; k++)
                {
                    if (mapa.mapa[k][j] == TRAMPA)
                    {
                        es_valido = true;

                        for (t=i+1; t<k; t++)
                        {
                            if (mapa.mapa[t][j] != 0)
                            {
                                es_valido = false;
                                break;
                            }
                        }

                        if (es_valido && cantidad < max_rayos)
                        {
                            rayos[cantidad].origen.x = (j + 0.5f) * mapa.ancho_bloque;
                            rayos[cantidad].origen.y = (i + 0.5f) * mapa.alto_bloque;
                            rayos[cantidad].objetivo.x = (j + 0.5f) * mapa.ancho_bloque;
                            rayos[cantidad].objetivo.y = (k + 0.5f) * mapa.alto_bloque;
                            rayos[cantidad].velocidad.y = (rayos[cantidad].objetivo.y - rayos[cantidad].origen.y) / TIEMPO_RAYO_EN_APARICION;  // Velocidad vertical del rayo
                            cantidad++;
                        }

                        break;
                    }
                }
            }
        }
    }

    return cantidad;
}


/**
 * Función que dibuja un rayo.
 * @param rayo El rayo que se quiere dibujar.
 * @param color Color del cual se desea pintar el 
 */
Procedure dibujar_rayo(Rayo rayo, ALLEGRO_COLOR color)
{
    ALLEGRO_VERTEX vertices[NRO_OSCILACIONES_RAYO];
    Natural i, nro_vertices;
    float delta_x, delta_y, distancia, t, offset;
    bool es_vertical;

    if (!rayo.activo)
    {
        return;  // Si el rayo no está activo, no se dibuja
    }
    
    delta_x = rayo.objetivo.x - rayo.origen.x;
    delta_y = rayo.objetivo.y - rayo.origen.y;
    distancia = sqrt(delta_x * delta_x + delta_y * delta_y);
    es_vertical = (fabs(delta_x) < TOLERANCIA);  // Verifica si el rayo es vertical

    nro_vertices = fmax(2, (Natural)(rayo.porcentaje_progreso / 100 * (NRO_OSCILACIONES_RAYO - 1)) + 1);  // Mínimo 2 vértices

    for (i=0; i<nro_vertices; i++)
    {
        t = (float) i / (NRO_OSCILACIONES_RAYO - 1);
        offset = AMPLITUD_OSCILACION_RAYO * sin(2*PI*t);  // Oscilación de 8 px

        if (es_vertical)
        {
            vertices[i].x = rayo.origen.x + offset;
            vertices[i].y = rayo.origen.y + t * distancia * rayo.porcentaje_progreso / 100;
        }

        else
        {
            vertices[i].x = rayo.origen.x + t * distancia * rayo.porcentaje_progreso / 100;
            vertices[i].y = rayo.origen.y + offset;
        }

        vertices[i].color = color;
    }

    al_draw_prim(vertices, NULL, NULL, 0, nro_vertices, ALLEGRO_PRIM_LINE_STRIP);
}


bool personaje_activa_rayo(Rayo rayo, Personaje personaje)
{
    float min_x = fmin(rayo.origen.x, rayo.objetivo.x);
    float max_x = fmax(rayo.origen.x, rayo.objetivo.x);
    float min_y = fmin(rayo.origen.y, rayo.objetivo.y);
    float max_y = fmax(rayo.origen.y, rayo.objetivo.y);

    // Si el rayo es vertical, revisamos si el personaje está en la misma zona vertical
    if (fabs(rayo.origen.x - rayo.objetivo.x) < TOLERANCIA)
    {
        return (personaje.posicion.y + personaje.alto >= min_y && personaje.posicion.y <= max_y);
    }

    
    else  // Si el rayo es horizontal, revisamos si el personaje está en la misma zona horizontal
    {
        return (personaje.posicion.x + personaje.ancho >= min_x && personaje.posicion.x <= max_x);
    }
}


Procedure actualizar_rayo(Rayo* rayo, Personaje personaje)
{
    rayo->tiempo_en_etapa += 1./FPS;

    switch (rayo->etapa)
    {
        case INOPERATIVO:

            rayo->activo = false;

            if (personaje_activa_rayo(*rayo, personaje))
            {
                rayo->etapa = EN_ESPERA;
                rayo->tiempo_en_etapa = 0;
            }

        case EN_ESPERA:

            rayo->activo = false;

            if (rayo->tiempo_en_etapa >= TIEMPO_RAYO_EN_ESPERA)
            {
                rayo->etapa = EN_APARICION;
                rayo->tiempo_en_etapa = 0;
            }

            break;


        case EN_APARICION:
        {
            rayo->activo = true;
            rayo->porcentaje_progreso = rayo->tiempo_en_etapa / TIEMPO_RAYO_EN_APARICION;

            printf("Estoy listo para atacar.\n");

            if (rayo->porcentaje_progreso >= 100)
            {
                rayo->etapa = ACTIVO_AL_100;
                rayo->tiempo_en_etapa = 0;
                rayo->porcentaje_progreso = 100;
            }

            break;
        }

        case ACTIVO_AL_100:

            if (rayo->tiempo_en_etapa >= TIEMPO_RAYO_ACTIVO_AL_100)
            {
                rayo->etapa = EN_DESAPARICION;
                rayo->tiempo_en_etapa = 0;
            }

            break;

        case EN_DESAPARICION:
        {
            rayo->porcentaje_progreso = 100 * (1 - rayo->tiempo_en_etapa / TIEMPO_RAYO_EN_DESAPARICION);

            if (rayo->porcentaje_progreso <= 0)
            {
                rayo->etapa = EN_ESPERA;
                rayo->tiempo_en_etapa = 0;
                rayo->porcentaje_progreso = 0;
            }

            break;
        }
    }

    if (rayo->activo)
    {
        dibujar_rayo(*rayo, AMARILLO);
    }
}
