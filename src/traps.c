#include "traps.h"

/**
 * Función que inicializa un rayo con sus valores por defecto.
 * @param rayo Puntero al rayo a inicializar.
 * @param efecto Puntero al efecto de sonido del rayo.
 */
Procedure inicializar_rayo(Rayo* rayo, EfectoSonido* efecto)
{
    rayo->origen.x = 0;
    rayo->origen.y = 0;
    rayo->objetivo.x = 0;
    rayo->objetivo.y = 0;
    rayo->posicion.x = 0;
    rayo->posicion.y = 0;
    rayo->grosor = GROSOR_INICIAL_RAYO;
    rayo->danho = DANHO_RAYO;  // Asigna el daño del rayo
    rayo->activo = false;  // Inicialmente el rayo no está activo
    rayo->etapa = INOPERATIVO;
    rayo->porcentaje_progreso = 0;  // Inicializa el porcentaje de progreso a 0
    rayo->tiempo_en_etapa = 0;  // Inicializa el tiempo en etapa a 0
    rayo->efecto_sonido = efecto;  // Se asigna el efecto de sonido del rayo
    rayo->efecto_sonido_ya_empezado = false;
    rayo->efecto_sonido_ya_detenido = false;
}


/**
 * Función para inicializar los rayos de todos los niveles.
 * @param rayos La matriz con los rayos de los niveles.
 * @param efecto El efecto de sonido del rayo.
 */
Procedure inicializar_rayos(Rayo rayos[NRO_NIVELES][MAX_RAYOS], EfectoSonido* efecto)
{
    Natural i, j;

    for (i=0; i<NRO_NIVELES; i++)
    {
        for (j=0; j<MAX_RAYOS; j++)
        {
            inicializar_rayo(&rayos[i][j], efecto);
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
            if (mapa.mapa[i][j] == BLOQUE_RAYO)
            {
                // Buscar en dirección horizontal
                for (k=j+1; k<mapa.nro_columnas; k++)
                {
                    if (mapa.mapa[i][k] == BLOQUE_RAYO)
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
                            rayos[cantidad].origen.x = (j + 0.5f) * mapa.ancho_bloque + RADIO_CIRCULO_ROJO;
                            rayos[cantidad].origen.y = (i + 0.5f) * mapa.alto_bloque;
                            rayos[cantidad].objetivo.x = (k + 0.5f) * mapa.ancho_bloque - RADIO_CIRCULO_ROJO;
                            rayos[cantidad].objetivo.y = (i + 0.5f) * mapa.alto_bloque;
                            rayos[cantidad].posicion = rayos[cantidad].origen;
                            cantidad++;
                        }

                        break;  // Solo detectamos el primer par
                    }
                }

                // Buscar en dirección vertical
                for (k=i+1; k<mapa.nro_filas; k++)
                {
                    if (mapa.mapa[k][j] == BLOQUE_RAYO)
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
                            rayos[cantidad].origen.y = (i + 0.5f) * mapa.alto_bloque + RADIO_CIRCULO_ROJO;
                            rayos[cantidad].objetivo.x = (j + 0.5f) * mapa.ancho_bloque;
                            rayos[cantidad].objetivo.y = (k + 0.5f) * mapa.alto_bloque - RADIO_CIRCULO_ROJO;
                            rayos[cantidad].posicion = rayos[cantidad].origen;
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
 * @param color Color del cual se desea pintar el rayo.
 */
Procedure dibujar_rayo(Rayo rayo, ALLEGRO_COLOR color)
{
    al_draw_line(rayo.origen.x, rayo.origen.y, rayo.posicion.x, rayo.posicion.y, color, rayo.grosor);  // Se dibuja la línea
}


bool linea_de_vision_libre(Rayo rayo, Personaje personaje, Mapa mapa)
{
    Natural col_min, col_max, fil_min, fil_max, col, fil;
    Natural col_personaje = (personaje.posicion.x + personaje.ancho / 2) / mapa.ancho_bloque;
    Natural fil_personaje = (personaje.posicion.y + personaje.alto / 2) / mapa.alto_bloque;
    Natural col_rayo = rayo.origen.x / mapa.ancho_bloque;
    Natural fil_rayo = rayo.origen.y / mapa.alto_bloque;

    if (rayo.origen.y == rayo.objetivo.y)  // Rayo horizontal
    {
        // Verificamos si la columna del personaje está entre el origen y el objetivo
        col_min = fmin(rayo.origen.x, rayo.objetivo.x) / mapa.ancho_bloque;
        col_max = fmax(rayo.origen.x, rayo.objetivo.x) / mapa.ancho_bloque;

        if (col_personaje >= col_min && col_personaje <= col_max)
        {
            // Revisar si hay BLOQUE en la columna del personaje entre su fila y la del rayo
            fil_min = fmin(fil_personaje, fil_rayo);
            fil_max = fmax(fil_personaje, fil_rayo);

            for (fil=fil_min+1; fil<fil_max; fil++)
            {
                if (mapa.mapa[fil][col_personaje] == BLOQUE)
                {
                    return false;  // Hay un obstáculo verticalmente entre personaje y rayo
                }
            }

            return true;  // No hay obstáculos en la línea de visión vertical
        }
    }

    else if (rayo.origen.x == rayo.objetivo.x)  // Rayo vertical
    {
        fil_min = fmin(rayo.origen.y, rayo.objetivo.y) / mapa.alto_bloque;
        fil_max = fmax(rayo.origen.y, rayo.objetivo.y) / mapa.alto_bloque;

        if (fil_personaje >= fil_min && fil_personaje <= fil_max)
        {
            // Revisar si hay BLOQUE en la fila del personaje entre su columna y la del rayo
            col_min = fmin(col_personaje, col_rayo);
            col_max = fmax(col_personaje, col_rayo);

            for (col=col_min+1; col<col_max; col++)
            {
                if (mapa.mapa[fil_personaje][col] == BLOQUE)
                {
                    return false;  // Hay un obstáculo horizontalmente entre personaje y rayo
                }
            }

            return true;  // No hay obstáculos en la línea de visión horizontal
        }
    }

    return false;  // No alineado o fuera del rango de trayectoria
}


bool personaje_activa_rayo(Rayo rayo, Personaje personaje, Mapa mapa)
{
    float min_x = fmin(rayo.origen.x, rayo.objetivo.x);
    float max_x = fmax(rayo.origen.x, rayo.objetivo.x);
    float min_y = fmin(rayo.origen.y, rayo.objetivo.y);
    float max_y = fmax(rayo.origen.y, rayo.objetivo.y);

    bool dentro_de_rango = false;
    bool resultado;

    // Si el rayo es vertical, revisamos si el personaje está en la misma zona vertical
    if (rayo.origen.x == rayo.objetivo.x)
    {
        dentro_de_rango = personaje.posicion.y + personaje.alto >= min_y && personaje.posicion.y <= max_y;
    }

    
    else  // Si el rayo es horizontal, revisamos si el personaje está en la misma zona horizontal
    {
        dentro_de_rango = personaje.posicion.x + personaje.ancho >= min_x && personaje.posicion.x <= max_x;
    }

    resultado = dentro_de_rango && linea_de_vision_libre(rayo, personaje, mapa);

    return resultado; // dentro_de_rango && linea_de_vision_libre(rayo, personaje, mapa);
}


Procedure actualizar_rayo(Rayo* rayo, Natural index, Personaje personaje, Mapa mapa)
{
    ALLEGRO_COLOR color_rayo;
    float tiempo_total = 0, parametro;
    bool proximidad = personaje_activa_rayo(*rayo, personaje, mapa);    

    rayo->tiempo_en_etapa += 1./FPS;

    switch (rayo->etapa)
    {
        case INOPERATIVO:

            rayo->activo = false;

            if (proximidad)
            {
                rayo->etapa = EN_APARICION;
                rayo->tiempo_en_etapa = 0;
            }

            break;

        case EN_APARICION:
        {
            rayo->activo = true;
            rayo->porcentaje_progreso = 100 * rayo->tiempo_en_etapa / TIEMPO_RAYO_EN_APARICION;
            tiempo_total = rayo->tiempo_en_etapa;

            if (rayo->porcentaje_progreso >= 100)
            {
                rayo->etapa = ACTIVO_AL_100;
                rayo->tiempo_en_etapa = 0;
                rayo->porcentaje_progreso = 100;
            }

            break;
        }

        case ACTIVO_AL_100:

            tiempo_total = TIEMPO_RAYO_EN_APARICION + rayo->tiempo_en_etapa;

            if (rayo->tiempo_en_etapa >= TIEMPO_RAYO_ACTIVO_AL_100)
            {
                rayo->etapa = EN_DESAPARICION;
                rayo->tiempo_en_etapa = 0;
            }

            break;

        case EN_DESAPARICION:
        {
            rayo->porcentaje_progreso = 100 * (1 - rayo->tiempo_en_etapa / TIEMPO_RAYO_EN_DESAPARICION);
            tiempo_total = TIEMPO_RAYO_EN_APARICION + TIEMPO_RAYO_ACTIVO_AL_100 + rayo->tiempo_en_etapa;

            if (rayo->porcentaje_progreso <= 0)
            {
                rayo->etapa = EN_ESPERA;
                rayo->tiempo_en_etapa = 0;
                rayo->porcentaje_progreso = 0;
            }

            break;
        }

        case EN_ESPERA:

            rayo->activo = false;
            tiempo_total = 0;

            if (rayo->tiempo_en_etapa >= TIEMPO_RAYO_EN_ESPERA)
            {
                rayo->etapa = EN_APARICION;
                rayo->tiempo_en_etapa = 0;
            }

            break;
    }

    if (rayo->origen.x == rayo->objetivo.x)  // Rayo vertical
    {
        rayo->posicion.y = rayo->origen.y + rayo->porcentaje_progreso / 100 * (rayo->objetivo.y - rayo->origen.y);
    }

    else  // Rayo horizontal
    {
        rayo->posicion.x = rayo->origen.x + rayo->porcentaje_progreso / 100 * (rayo->objetivo.x - rayo->origen.x);
    }

    if (rayo->activo)
    {
        parametro = 0.5 * (1 + sin(20*tiempo_total));
        rayo->grosor = GROSOR_INICIAL_RAYO + 0.75 * GROSOR_INICIAL_RAYO * parametro * pow(-1, rand() % 2);
        color_rayo = al_map_rgb((1-parametro)*255, 255, parametro*255);
        dibujar_rayo(*rayo, color_rayo);

        if (!rayo->efecto_sonido_ya_empezado)
        {
            al_set_sample_instance_gain(rayo->efecto_sonido->instancias[index], 3.5);  // Volumen
            al_set_sample_instance_pan(rayo->efecto_sonido->instancias[index], 0.0);  // Centro
            al_set_sample_instance_speed(rayo->efecto_sonido->instancias[index], 1.15);  // Velocidad 
            al_set_sample_instance_playmode(rayo->efecto_sonido->instancias[index], ALLEGRO_PLAYMODE_LOOP);  // Se repite si es que no se alcanza a reproducir completo
            al_play_sample_instance(rayo->efecto_sonido->instancias[index]);
            rayo->efecto_sonido_ya_empezado = true;
            rayo->efecto_sonido_ya_detenido = false;
        }
    }

    else
    {
        if (!rayo->efecto_sonido_ya_detenido)
        {
            al_stop_sample_instance(rayo->efecto_sonido->instancias[index]);
            rayo->efecto_sonido_ya_empezado = false;
            rayo->efecto_sonido_ya_detenido = true;
        }
    }

    if (!proximidad)
    {
        rayo->etapa = INOPERATIVO;
        rayo->tiempo_en_etapa = 0;
        rayo->porcentaje_progreso = 0;
    }
}


Procedure actualizar_rayos(Rayo rayos[MAX_RAYOS], Natural cantidad, Personaje personaje, Mapa mapa)
{
    Natural i;

    if (cantidad > MAX_RAYOS)
    {
        return;
    }

    for (i=0; i<cantidad; i++)
    {
        actualizar_rayo(&rayos[i], i, personaje, mapa);
    }
}

Procedure detectar_si_personaje_en_zona_de_espina(Personaje* personaje, Espina espina[MAX_ESPINAS])
{
    
}

/*
bool hay_hitbox_con_espina(Personaje* personaje, Mapa mapa, Espina* espina_detectada)
{
    float x1, y1, x2, y2, m, n, pos;
    Natural bloque_x1, bloque_y1, bloque_x2, bloque_y2;
    Vector pos_inicial_espina, pos_final_espina;
    bool espina_valida, colision;
    Natural i, j;

    x1 = personaje->posicion.x;
    x2 = personaje->posicion.x + personaje->ancho - 1;

    y1 = personaje->posicion.y;
    y2 = personaje->posicion.y + personaje->alto - 1;

    bloque_x1 = (Natural) x1 / mapa.ancho_bloque;
    bloque_x2 = (Natural) x2 / mapa.ancho_bloque;

    bloque_y1 = (Natural) y1 / mapa.alto_bloque;
    bloque_y2 = (Natural) y2 / mapa.alto_bloque;

    colision = false;

    for (i=bloque_x1; i<=bloque_x2; i++)
    {
        for (j=bloque_y1; j<=bloque_y2; j++)
        {
            if (mapa.mapa[i][j] == ESPINA)
            {
                espina_valida = true;

                if (j == mapa.nro_filas-1 || (j < mapa.nro_filas-1 && mapa.mapa[j+1][i] == BLOQUE))
                {
                    
                    espina_detectada->direccion_espina = 'I';  // Inferior
                    espina_detectada->posicion_vertice.x = (i + 0.5f) * mapa.ancho_bloque;
                    espina_detectada->posicion_vertice.y = (j + 1) * mapa.alto_bloque - espina_detectada->alto;
                    pos_inicial_espina.x = espina_detectada->posicion_vertice.x - espina_detectada->ancho / 2;
                    pos_inicial_espina.y = (j + 1) * mapa.alto_bloque;
                    pos_final_espina.x = espina_detectada->posicion_vertice.x + espina_detectada->ancho / 2;
                    pos_final_espina.y = (j + 1) * mapa.alto_bloque;
                }
                

                else if (j == 0 || (j > 0 && mapa.mapa[j-1][i] == BLOQUE))
                {
                    
                    espina_detectada->direccion_espina = 'S';  // Superior
                    espina_detectada->posicion_vertice.x = (i + 0.5f) * mapa.ancho_bloque;
                    espina_detectada->posicion_vertice.y = j * mapa.alto_bloque + espina_detectada->alto;
                    pos_inicial_espina.x = espina_detectada->posicion_vertice.x - espina_detectada->ancho / 2;
                    pos_inicial_espina.y = j * mapa.alto_bloque;
                    pos_final_espina.x = espina_detectada->posicion_vertice.x + espina_detectada->ancho / 2;
                    pos_final_espina.y = j * mapa.alto_bloque;
                }

                else if (i == 0 || (i > 0 && mapa.mapa[j][i-1] == BLOQUE))
                {
                    
                    espina_detectada->direccion_espina = 'L';  // Izquierda
                    espina_detectada->posicion_vertice.x = i * mapa.ancho_bloque + espina_detectada->alto;
                    espina_detectada->posicion_vertice.y = (j + 0.5f) * mapa.alto_bloque;
                    pos_inicial_espina.x = i * mapa.ancho_bloque;
                    pos_inicial_espina.y = espina_detectada->posicion_vertice.y - espina_detectada->ancho / 2;
                    pos_final_espina.x = i * mapa.ancho_bloque;
                    pos_final_espina.y = espina_detectada->posicion_vertice.y + espina_detectada->ancho / 2;
                }

                else
                {
                    if (i == mapa.nro_columnas-1 || (i < mapa.nro_columnas-1 && mapa.mapa[j][i+1]))
                    {
                        
                        espina_detectada->direccion_espina = 'L';
                        espina_detectada->posicion_vertice.x = (i+1) * mapa.ancho_bloque - espina_detectada->alto;
                        espina_detectada->posicion_vertice.y = (j + 0.5f) * mapa.alto_bloque;
                        pos_inicial_espina.x = (i+1) * mapa.ancho_bloque;
                        pos_inicial_espina.y = espina_detectada->posicion_vertice.y - espina_detectada->ancho/2;
                        pos_final_espina.x = (i+1) * mapa.ancho_bloque;
                        pos_final_espina.y = espina_detectada->posicion_vertice.y + espina_detectada->ancho/2;
                    }

                    else
                    {
                        espina_valida = false;
                    }
                }

                if (espina_valida)
                {
                    printf("Holi\n");
                    espina_detectada->regresiones[0].m = (espina_detectada->posicion_vertice.y - pos_inicial_espina.y) / (espina_detectada->posicion_vertice.x - pos_inicial_espina.x);
                    espina_detectada->regresiones[0].n = espina_detectada->posicion_vertice.y - espina_detectada->regresiones[0].m * espina_detectada->posicion_vertice.x;
                    espina_detectada->regresiones[1].m = (pos_final_espina.y - espina_detectada->posicion_vertice.y) / (pos_final_espina.x - espina_detectada->posicion_vertice.x);
                    espina_detectada->regresiones[1].n = espina_detectada->posicion_vertice.y - espina_detectada->regresiones[1].m * espina_detectada->posicion_vertice.x;

                    switch (espina_detectada->direccion_espina)
                    {
                        case 'I':  // Inferior

                            if ((x1 > pos_inicial_espina.x && x1 < pos_final_espina.x) || (x2 >= pos_inicial_espina.x && x2 <= pos_final_espina.x))
                            {
                                for (pos=x1; !colision && pos<x2; pos++)
                                {
                                    if (pos <= espina_detectada->posicion_vertice.x)
                                    {
                                        m = espina_detectada->regresiones[0].m;
                                        n = espina_detectada->regresiones[0].n;
                                    }

                                    else
                                    {
                                        m = espina_detectada->regresiones[1].m;
                                        n = espina_detectada->regresiones[1].n;
                                    }
                                    
                                    if (y2 >= m*pos + n && y2 <= pos_inicial_espina.y)
                                    {
                                        colision = true;
                                    }
                                }
                            }

                            break;

                        case 'S':  // Superior
                        
                            if ((x1 > pos_inicial_espina.x && x1 < pos_final_espina.x) || (x2 >= pos_inicial_espina.x && x2 <= pos_final_espina.x))
                            {
                                for (pos=x1; !colision && pos<x2; pos++)
                                {
                                    if (pos <= espina_detectada->posicion_vertice.x)
                                    {
                                        m = espina_detectada->regresiones[0].m;
                                        n = espina_detectada->regresiones[0].n;
                                    }

                                    else
                                    {
                                        m = espina_detectada->regresiones[1].m;
                                        n = espina_detectada->regresiones[1].n;
                                    }
                                    
                                    if (y1 < m*pos + n && y1 >= pos_inicial_espina.y)
                                    {
                                        colision = true;
                                    }
                                }
                            }

                            break;

                        case 'L':  // Izquierda (left)
                            
                            if ((y1 > pos_inicial_espina.y && y1 < pos_final_espina.y) || (y2 >= pos_inicial_espina.y && y2 <= pos_final_espina.y))
                            {
                                for (pos=y1; !colision && pos<y2; pos++)
                                {
                                    if (pos <= espina_detectada->posicion_vertice.y)
                                    {
                                        m = espina_detectada->regresiones[0].m;
                                        n = espina_detectada->regresiones[0].n;
                                    }

                                    else
                                    {
                                        m = espina_detectada->regresiones[1].m;
                                        n = espina_detectada->regresiones[1].n;
                                    }
                                    
                                    if (x1 < (1./m)*pos - n/m && x1 >= pos_inicial_espina.x)
                                    {
                                        colision = true;                
                                    }
                                }
                            }

                            break;

                        default:  // Caso derecho (right = 'R')
                            
                            printf("Holo\n");
                            if ((y1 > pos_inicial_espina.y && y1 < pos_final_espina.y) || (y2 >= pos_inicial_espina.y && y2 <= pos_final_espina.y))
                            {
                                for (pos=y1; !colision && pos<y2; pos++)
                                {
                                    if (pos <= espina_detectada->posicion_vertice.y)
                                    {
                                        m = espina_detectada->regresiones[0].m;
                                        n = espina_detectada->regresiones[0].n;
                                    }

                                    else
                                    {
                                        m = espina_detectada->regresiones[1].m;
                                        n = espina_detectada->regresiones[1].n;
                                    }
                                    
                                    if (x2 >= (1./m)*pos - n/m && x2 <= pos_final_espina.x)
                                    {
                                        colision = true;
                                        personaje->hay_obj_der = true;
                                    }
                                }
                            }

                            break;
                    }

                    if (colision)
                    {
                        printf("Holu\n");
                        return true;
                    }
                }
            }
        }
    }

    personaje->hay_obj_izq = false;
    personaje->hay_obj_der = false;
    personaje->hay_obj_sup = false;

    return false;
}
*/

