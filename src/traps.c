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
                            if (mapa.mapa[i][t] == BLOQUE)
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
                            if (mapa.mapa[t][j] == BLOQUE)
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
                if (mapa.mapa[fil][col_personaje] == BLOQUE || mapa.mapa[fil][col_personaje] == BLOQUE_RAYO)
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
                if (mapa.mapa[fil_personaje][col] == BLOQUE || mapa.mapa[fil_personaje][col] == BLOQUE_RAYO)
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

Procedure detectar_si_personaje_en_zona_de_espina(Personaje* personaje, Espina espinas[MAX_ESPINAS])
{
    Natural i, index_regresion;

    for (i=0; i<MAX_ESPINAS; i++)
    {
        if (espinas[i].activa)
        {
            switch (espinas[i].direccion_espina)
            {
                case 'I':  // Para la espina inferior

                    index_regresion = personaje->posicion.x + 0.5f * personaje->ancho > espinas[i].posicion_vertice.x ? 1 : 0;

                    if (personaje->posicion.x + 0.9f * personaje->ancho > espinas[i].posicion_vertice.x - espinas[i].ancho/2 && 
                        personaje->posicion.x + 0.1f * personaje->ancho < espinas[i].posicion_vertice.x + espinas[i].ancho/2 &&
                        personaje->posicion.y < espinas[i].posicion_vertice.y + espinas[i].alto && 
                        personaje->posicion.y + personaje->alto > espinas[i].regresiones[index_regresion].m * (personaje->posicion.x + 0.5f * personaje->ancho) + espinas[i].regresiones[index_regresion].n)
                    {
                        if (!personaje->danhado)
                        {
                            personaje->danhado = true;
                            aplicar_danho(personaje, DANHO_ESPINA);
                        }
                    }

                    break;
                
                case 'S':  // Para la espina superior

                    index_regresion = personaje->posicion.x + 0.5f * personaje->ancho > espinas[i].posicion_vertice.x ? 1 : 0;

                    if (personaje->posicion.x + 0.9f * personaje->ancho > espinas[i].posicion_vertice.x - espinas[i].ancho/2 && 
                        personaje->posicion.x + 0.1f * personaje->ancho < espinas[i].posicion_vertice.x + espinas[i].ancho/2 && 
                        personaje->posicion.y + personaje->alto > espinas[i].posicion_vertice.y - espinas[i].alto &&
                        personaje->posicion.y < espinas[i].regresiones[index_regresion].m * (personaje->posicion.x + 0.5f * personaje->ancho) + espinas[i].regresiones[index_regresion].n)
                    {
                        if (!personaje->danhado)
                        {
                            personaje->danhado = true;
                            aplicar_danho(personaje, DANHO_ESPINA);
                        }
                    }

                    break;

                case 'L':  // Para la espina izquierda
                    
                    index_regresion = personaje->posicion.y + 0.5f * personaje->alto > espinas[i].posicion_vertice.y ? 1 : 0;

                    if (personaje->posicion.y + 0.9f * personaje->alto > espinas[i].posicion_vertice.y - espinas[i].ancho/2 &&
                        personaje->posicion.y + 0.1f * personaje->alto < espinas[i].posicion_vertice.y + espinas[i].ancho/2 &&
                        personaje->posicion.x > espinas[i].posicion_vertice.x - espinas[i].alto &&
                        personaje->posicion.x < 1./espinas[i].regresiones[index_regresion].m * (personaje->posicion.y + 0.5f * personaje->alto - espinas[i].regresiones[index_regresion].n))
                    {
                        if (!personaje->danhado)
                        {
                            personaje->danhado = true;
                            aplicar_danho(personaje, DANHO_ESPINA);
                        }
                    }

                    break;

                case 'R':  // Para la espina derecha

                    index_regresion = personaje->posicion.y + 0.5f * personaje->alto > espinas[i].posicion_vertice.y ? 1 : 0;

                    if (personaje->posicion.y + 0.9f * personaje->alto > espinas[i].posicion_vertice.y - espinas[i].ancho/2 &&
                        personaje->posicion.y + 0.1f * personaje->alto < espinas[i].posicion_vertice.y + espinas[i].ancho/2 &&
                        personaje->posicion.x < espinas[i].posicion_vertice.x + espinas[i].alto &&
                        personaje->posicion.x + personaje->ancho > 1./espinas[i].regresiones[index_regresion].m * (personaje->posicion.y + 0.5f * personaje->alto - espinas[i].regresiones[index_regresion].n))
                    {
                        if (!personaje->danhado)
                        {
                            personaje->danhado = true;
                            aplicar_danho(personaje, DANHO_ESPINA);
                        }
                    }

                    break;
            }
        }
    }
}


Procedure detectar_si_personaje_en_zona_de_veneno(Personaje* personaje, Charco charcos[MAX_CHARCOS], Natural nivel)
{
    Natural i;
    Natural base_disparo = NRO_FRAMES_MOVIMIENTO + NRO_FRAMES_PELEA;
    float porcentajes_colision_min[NRO_FRAMES_DISPARO] = {10.7, 21.8, 17.3, 25.3, 17.5, 14.7};
    float porcentajes_colision_max[NRO_FRAMES_DISPARO] = {56.0, 52.6, 65.3, 49.3, 73.0, 57.3};

    for (i=0; i<MAX_CHARCOS; i++)
    {
        if (charcos[i].activo)
        {
            if (nivel < 3)
            {
                if (personaje->posicion.x + 0.9f*personaje->ancho > charcos[i].posicion.x && personaje->posicion.x + 0.1f*personaje->ancho < charcos[i].posicion.x + charcos[i].ancho)
                {
                    if (fabs(personaje->posicion.y + personaje->alto - charcos[i].posicion.y) < 2)
                    {
                        if (!personaje->danhado)
                        {
                            personaje->danhado = true;
                            aplicar_danho(personaje, 100);
                        }
                    }
                }
            }

            else
            {    
                if (personaje->direccion == 1)
                {
                    if (personaje->posicion.x + porcentajes_colision_max[personaje->id_nro_frame - base_disparo]/100 * personaje->ancho > charcos[i].posicion.x &&
                        personaje->posicion.x + porcentajes_colision_min[personaje->id_nro_frame - base_disparo]/100 * personaje->ancho < charcos[i].posicion.x + charcos[i].ancho)
                    {
                        if (fabs(personaje->posicion.y + personaje->alto - charcos[i].posicion.y) < 2)
                        {
                            if (!personaje->danhado)
                            {
                                personaje->danhado = true;
                                aplicar_danho(personaje, 100);
                            }
                        }
                    }
                }

                else
                {
                    if (personaje->posicion.x + (1 - porcentajes_colision_max[personaje->id_nro_frame - base_disparo]/100) * personaje->ancho < charcos[i].posicion.x + charcos[i].ancho &&
                        personaje->posicion.x + (1 - porcentajes_colision_min[personaje->id_nro_frame - base_disparo]/100) * personaje->ancho > charcos[i].posicion.x)
                    {
                        if (fabs(personaje->posicion.y + personaje->alto - charcos[i].posicion.y) < 2)
                        {
                            if (!personaje->danhado)
                            {
                                personaje->danhado = true;
                                aplicar_danho(personaje, 100);
                            }
                        }
                    }
                }   
            }
        }
    }
}