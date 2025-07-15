#include "characters.h"

/**
 * Función que inicializa un personaje según su tipo.
 * @param personaje Puntero al personaje a inicializar.
 * @param tipo Tipo de personaje a inicializar ('W' para Woofson, 'D' para Dragón, 'H' para Humanoide, 'J' para Jefe).
 * @return true si se inicializó correctamente, false en caso contrario.
 */
bool inicializar_personaje(Personaje* personaje, char tipo)
{
    Natural i;
    char ruta_imagen[50] = {'\0'};

    for (i=0; i<NRO_FRAMES; i++)
    {
        personaje->frames[i] = NULL;  // Inicializa cada frame a NULL
    }

    switch (tipo)
    {
        case 'W':
            personaje->tipo = 'W';  // Woofson
            personaje->velocidad.x = 0;
            personaje->velocidad.y = 0;  // Inicialmente no está en salto
            personaje->caminata = false;  // Inicialmente no está en movimiento
            personaje->fps_en_caminata = 0;  // Inicializa el tiempo de caminata a 0

            for (i=0; i<NRO_FRAMES; i++)
            {
                sprintf(ruta_imagen, "assets/images/woofson_frames/woofson-%hu.png", i+1);
                personaje->frames[i] = al_load_bitmap(ruta_imagen);

                if (!personaje->frames[i])
                {
                    printf("Error al cargar la imagen del frame %hu del personaje Woofson.\n", i+1);
                    return false;
                }
            }

            personaje->imagen = personaje->frames[0];
            personaje->id_nro_frame = 0;  // Inicializa el número de frame actual a 0
            personaje->ancho = al_get_bitmap_width(personaje->imagen);  // Ancho del personaje
            personaje->nro_vidas = VIDAS_INICIALES;
            personaje->subvida_actual = 100;
            personaje->ancho = al_get_bitmap_width(personaje->imagen);  // Ancho del personaje
            personaje->alto = al_get_bitmap_height(personaje->imagen);  // Alto del personaje
            personaje->posicion.x = ANCHO_VENTANA * 0.1;
            personaje->posicion.y = ALTURA_PISO - personaje->alto;  // Se coloca en el piso
            personaje->bandera_dibujo = 0;  // 0: normal, ALLEGRO_FLIP_HORIZONTAL: espejo
            personaje->en_plataforma = false;  // Inicialmente no está en una plataforma (está en el suelo)
            personaje->danhado = false;  // Inicialmente no recibe ningún tipo de danho
            personaje->tiempo_danho = 0;
            inicializar_salto(personaje);  // Inicializa la estructura del salto para el personaje
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

    return true;
}


/**
 * Función que inicializa la estructura de salto del personaje.
 * @param personaje Puntero al personaje cuya estructura de salto se va a inicializar.
 */
Procedure inicializar_salto(Personaje* personaje)
{
    personaje->salto.en_salto = false;
    personaje->salto.es_interrumpido = false;  // Indica si el salto es interrumpido por una colisión con algún techo o no
    personaje->salto.tiempo_en_salto = 0;  // Reinicia el tiempo de salto
    personaje->salto.impulso = IMPULSO_PERSONAJE;  // Si bien no es necesario, es una buena práctica para la legibilidad
    personaje->salto.altura_inicial = personaje->posicion.y;  // Reinicia la altura inicial del salto
    return;
}


/**
 * Función que determina cómo se debe dibujar el personaje según las teclas presionadas.
 * @param personaje El personaje a dibujar.
 * @param teclas Un arreglo de booleanos que indica qué teclas están presionadas.
 */
Procedure determinar_como_dibujar_personaje(Personaje* personaje, Natural ultima_tecla_lateral)
{
    extern bool teclas[ALLEGRO_KEY_MAX];  // Arreglo global de teclas presionadas

    if (teclas[ALLEGRO_KEY_LEFT] || (!teclas[ALLEGRO_KEY_RIGHT] && ultima_tecla_lateral == ALLEGRO_KEY_LEFT))
    {
        personaje->bandera_dibujo = ALLEGRO_FLIP_HORIZONTAL;  // Dibuja el personaje mirando a la izquierda
    }

    else
    {
        personaje->bandera_dibujo = 0;  // Dibuja el personaje mirando a la derecha
    }
}


/**
 * Función que dibuja el personaje en su posición actual.
 * @param personaje El personaje a dibujar.
 * @param teclas Un arreglo de booleanos que indica qué teclas están presionadas.
 * @param ultima_tecla_lateral La última tecla lateral presionada (para determinar la dirección de dibujo).
 */
Procedure dibujar_personaje(Personaje personaje, Natural ultima_tecla_lateral)
{
    extern bool teclas[ALLEGRO_KEY_MAX];  // Arreglo global de teclas presionadas
    determinar_como_dibujar_personaje(&personaje, ultima_tecla_lateral);
    al_draw_bitmap(personaje.imagen, personaje.posicion.x, personaje.posicion.y, personaje.bandera_dibujo);
}


/**
 * Función que actualiza el frame del personaje según su velocidad y estado de caminata.
 * @param personaje Puntero al personaje cuyo frame se va a actualizar.
 * @param modo Modo de actualización del frame ('M' para movimiento, 'P' para pelea).
 */
Procedure actualizar_frame(Personaje* personaje, char modo)
{
    if (modo == 'P')  // Si está en modo pelea, se actualiza el frame de pelea
    {
        personaje->id_nro_frame = NRO_FRAMES_MOVIMIENTO + (personaje->id_nro_frame + 1) % NRO_FRAMES_PELEA ;  // Evita que el frame se salga del rango
    }

    else  // Si está en modo movimiento, se actualiza el frame de movimiento siempre que esté en movimiento o en pataleo
    {
        if (personaje->velocidad.x != 0)
        {
            personaje->id_nro_frame = (personaje->id_nro_frame + 1) % NRO_FRAMES_MOVIMIENTO;  // Evita que el frame se salga del rango
        }
    }

    if (personaje->frames[personaje->id_nro_frame])  // Verifica si hay un frame siguiente para evitar errores de acceso a memoria
    {
        personaje->imagen = personaje->frames[personaje->id_nro_frame];  // Cambia al siguiente frame de animación
        personaje->ancho = al_get_bitmap_width(personaje->imagen);  // Actualiza el ancho del personaje (el alto no cambia)
    }
}


/**
 * Función que verifica si el personaje puede moverse lateralmente según las teclas presionadas y el mapa.
 * @param personaje El personaje a verificar.
 * @param teclas Un arreglo de booleanos que indica qué teclas están presionadas.
 * @param mapa El mapa del juego, que contiene los bloques y obstáculos.
 * @return true si el personaje puede moverse lateralmente, false en caso contrario.
 */
bool es_posible_mover_personaje_lateralmente(Personaje *personaje, Mapa mapa)
{
    extern bool teclas[ALLEGRO_KEY_MAX];  // Arreglo global de teclas presionadas

    if (teclas[ALLEGRO_KEY_LEFT] && !hay_colision_izquierda(personaje, mapa))
    {
        return true;  // Puede moverse a la izquierda
    }

    else if (teclas[ALLEGRO_KEY_RIGHT] && !hay_colision_derecha(personaje, mapa))
    {
        return true;  // Puede moverse a la derecha
    }

    return false;  // No puede moverse lateralmente
}


 /**
 * Función para plasmar la lógica del movimiento del personaje según las teclas presionadas y el mapa.
 * También maneja la lógica del salto y las colisiones con el mapa.
 * @param personaje Puntero al personaje que se va a mover.
 * @param teclas Un arreglo de booleanos que indica qué teclas están presionadas.
 * @param mapa El mapa del juego, que contiene los bloques y obstáculos.
 */
Procedure mover_personaje(Personaje* personaje, Mapa mapa)
{
    extern bool teclas[ALLEGRO_KEY_MAX];  // Arreglo global de teclas presionadas

    personaje->caminata = teclas[ALLEGRO_KEY_LEFT] ^ teclas[ALLEGRO_KEY_RIGHT];  // Actualiza el estado de caminata según las teclas presionadas
    
    if (teclas[ALLEGRO_KEY_SPACE])
    {
        personaje->fps_en_pelea++;

        if (personaje->fps_en_pelea % 8 == 0)  // Actualiza el frame de pelea cada 8 frames
        {
            actualizar_frame(personaje, 'P');  // Actualiza el frame del personaje si está en pelea
        }
    }
    
    if (personaje->caminata)
    {
        personaje->fps_en_caminata++;

        if (!teclas[ALLEGRO_KEY_SPACE])  // Si no está en pelea, actualiza el frame de caminata
        {
            if (personaje->velocidad.x != 0 && personaje->fps_en_caminata % (int) fabs((30 / ceilf(personaje->velocidad.x))) == 0)  // Actualiza el frame cada cierto numero de iteraciones que depende de la velocidad
            {
                actualizar_frame(personaje, 'M');  // Actualiza el frame del personaje si está caminando
            }
        }

        if (es_posible_mover_personaje_lateralmente(personaje, mapa))
        {
            if (fabs(personaje->velocidad.x) < VELOCIDAD_MAXIMA_PERSONAJE)
            {
                personaje->velocidad.x = teclas[ALLEGRO_KEY_LEFT] ? personaje->velocidad.x - ACELERACION_PERSONAJE 
                                                                  : personaje->velocidad.x + ACELERACION_PERSONAJE;   // Acelera el personaje al caminar dependiendo del sentido
            }

            else
            {
                personaje->velocidad.x = teclas[ALLEGRO_KEY_LEFT] ? -VELOCIDAD_MAXIMA_PERSONAJE 
                                                                  : VELOCIDAD_MAXIMA_PERSONAJE;  // Si el módulo de la velocidad es mayor a la máxima, la limita
            }
        }
    }

    else
    {
        if (fabs(personaje->velocidad.x) < ACELERACION_PERSONAJE)  // Si la velocidad es menor a la aceleración, se detiene
        {
            personaje->velocidad.x = 0;
            personaje->fps_en_caminata = 0;  // Reinicia el tiempo de caminata al detenerse
            
            if (!teclas[ALLEGRO_KEY_SPACE])
            {
                personaje->id_nro_frame = 0;  // Reinicia el frame del personaje al detenerse
                personaje->imagen = personaje->frames[0];  // Vuelve al primer frame de la animación
            }
        }

        else
        {
            personaje->fps_en_caminata++;
            
            if (!teclas[ALLEGRO_KEY_SPACE] && personaje->fps_en_caminata % (int) fabs((30 / ceilf(personaje->velocidad.x))) == 0)
            {
                actualizar_frame(personaje, 'M');  // Actualiza el frame del personaje si está caminando
            }
            
            personaje->velocidad.x = personaje->velocidad.x < 0 ? personaje->velocidad.x + ACELERACION_PERSONAJE 
                                                                : personaje->velocidad.x - ACELERACION_PERSONAJE;  // Disminuye la velocidad de movimiento del personaje al dejar de caminar
        }
    }

    personaje->posicion.x += personaje->velocidad.x;  // Actualiza la posición del personaje en el eje x

    if (teclas[ALLEGRO_KEY_UP] && !personaje->salto.en_salto && !hay_colision_superior(personaje, mapa))
    {
        personaje->salto.en_salto = true;  /* Activa el salto */
        personaje->salto.altura_inicial = personaje->posicion.y;  /* Guarda la altura inicial del salto */
        personaje->salto.tiempo_en_salto = 0;  /* Reinicia el tiempo de salto */
    }

    if (personaje->velocidad.x < 0 && !hay_colision_izquierda(personaje, mapa))
    {
        if (!personaje->salto.en_salto && personaje->posicion.y + personaje->alto < ALTURA_PISO && !hay_bloque_debajo(personaje, mapa))
        {
            activar_caida_libre(personaje);  /* Activa la caída libre si el personaje no está en el suelo */
        }
    }

    if (personaje->velocidad.x > 0 && !hay_colision_derecha(personaje, mapa))
    {
        if (!personaje->salto.en_salto && personaje->posicion.y + personaje->alto < ALTURA_PISO && !hay_bloque_debajo(personaje, mapa))
        {
            activar_caida_libre(personaje);  /* Activa la caída libre si el personaje no está en el suelo */
        }
    }
    
    if (personaje->salto.en_salto)  /* Si el personaje ya está en salto, se continúa el salto */
    {
        personaje->salto.tiempo_en_salto += 1.0 / FPS;
        continuar_salto(personaje, personaje->salto.tiempo_en_salto, mapa);
    }
    
    if (hay_colision_con_bordes(personaje, mapa))
    {
        efectuar_colision(personaje, mapa);
    }
}


/**
 * Función que actualiza la posición del personaje durante un salto.
 * @param personaje El personaje que está saltando.
 * @param t El tiempo transcurrido desde el inicio del salto.
 * @param mapa El mapa del juego, que contiene los bloques y obstáculos.
 */
Procedure continuar_salto(Personaje* personaje, float t, Mapa mapa)
{
    personaje->posicion.y = personaje->salto.altura_inicial - personaje->salto.impulso * t + g * t * t / 2;
    personaje->velocidad.y = velocidad_instantanea(*personaje, t);  /* Actualiza la velocidad en el eje y */

    if (hay_colision_superior(personaje, mapa))
    {
        efectuar_colision(personaje, mapa);
    }

    /* Si el personaje colisiona ya sea con el suelo o con algún bloque sólido u objeto, se detiene el salto */
    if (hay_colision_inferior(personaje, mapa))
    {
        efectuar_colision(personaje, mapa);  /* Efectúa la colisión con el suelo o bloque */
    }

    return;
}


/**
 * Función que activa la caída libre del personaje, actualizando su posición y velocidad en el eje y.
 * @param personaje El personaje que está cayendo.
 */
Procedure activar_caida_libre(Personaje* personaje)
{
    personaje->salto.en_salto = true;  /* Marca que el personaje está en caída libre */
    personaje->salto.altura_inicial = personaje->posicion.y;  /* Guarda la altura inicial de la caída */
    personaje->salto.tiempo_en_salto = 0;  /* Reinicia el tiempo de salto */
    personaje->salto.impulso = 0;  /* Parte con velocidad inicial 0 */
    return;
}


/**
 * Función que calcula la velocidad vertical del personaje en un instante de tiempo, teniendo en cuenta que v = dy/dt.
 * @param personaje Es el personaje del cual se quiere conocer su velocidad vertical.
 * @return La velocidad vertical del personaje.
 */
Entero velocidad_instantanea(Personaje personaje, float t)
{
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


/**
 * Función que hace patalear al personaje (se usa para las colisiones con bloques).
 * @param personaje El personaje que va a patalear.
 * @param direccion La dirección en la que se va a patalear (1 para derecha, -1 para izquierda).
 */
Procedure patalear(Personaje* personaje, int direccion)
{
    if (personaje->caminata)
    {
        if (fabs(personaje->velocidad.x) < VELOCIDAD_MAXIMA_PERSONAJE)
        {
            personaje->velocidad.x = personaje->velocidad.x + ACELERACION_PERSONAJE * direccion;  /* Se acelera el pataleo del personaje */   
        }

        else if (fabs(personaje->velocidad.x) > VELOCIDAD_MAXIMA_PERSONAJE)
        {
            personaje->velocidad.x = VELOCIDAD_MAXIMA_PERSONAJE * direccion;  /* Se limita la magnitud de la velocidad del pataleo */
        }
    }

    else
    {
        if (fabs(personaje->velocidad.x) < ACELERACION_PERSONAJE)
        {
            personaje->velocidad.x = 0;  /* Detiene la velocidad en el eje x */
        }

        else
        {
            personaje->velocidad.x = personaje->velocidad.x - ACELERACION_PERSONAJE * direccion;  /* Disminuye la magnitud de la velocidad de pataleo */
        }
    }
}


Procedure detectar_si_personaje_en_zona_de_rayo(Personaje* personaje, Rayo rayos[MAX_RAYOS])
{
    if (personaje->danhado)
    {
        personaje->tiempo_danho += 1.0 / FPS;

        if (personaje->tiempo_danho >= MAX_TIEMPO_INMUNE)
        {
            personaje->danhado = false;
            personaje->tiempo_danho = 0;
        }
        return;
    }

    float px1 = personaje->posicion.x;
    float py1 = personaje->posicion.y;
    float px2 = px1 + personaje->ancho;
    float py2 = py1 + personaje->alto;

    for (Natural i = 0; i < MAX_RAYOS; i++)
    {
        Rayo* rayo = &rayos[i];
        if (!rayo->activo)
            continue;

        debug_rayo_personaje(rayo, personaje);  // Imprime valores para debug

        if (rayo->origen.x == rayo->objetivo.x)  // Rayo vertical
        {
            float rx = rayo->origen.x;
            float y1 = fmin(rayo->origen.y, rayo->posicion.y);
            float y2 = fmax(rayo->origen.y, rayo->posicion.y);
            float rx1 = rx - GROSOR_RAYO / 2.0;
            float rx2 = rx + GROSOR_RAYO / 2.0;

            if (px2 > rx1 && px1 < rx2 && py2 > y1 && py1 < y2)
            {
                printf("¡Colisión vertical detectada!\n");
                personaje->danhado = true;
                personaje->tiempo_danho = 0;
                break;
            }
        }
        else  // Rayo horizontal
        {
            float ry = rayo->origen.y;
            float x1 = fmin(rayo->origen.x, rayo->posicion.x);
            float x2 = fmax(rayo->origen.x, rayo->posicion.x);
            float ry1 = ry - GROSOR_RAYO / 2.0;
            float ry2 = ry + GROSOR_RAYO / 2.0;

            if (py2 > ry1 && py1 < ry2 && px2 > x1 && px1 < x2)
            {
                printf("¡Colisión horizontal detectada!\n");
                personaje->danhado = true;
                personaje->tiempo_danho = 0;
                break;
            }
        }
    }

    if (personaje->danhado && personaje->tiempo_danho == 0)
    {
        personaje->subvida_actual = (personaje->subvida_actual <= DANHO_RAYO)
            ? 0
            : personaje->subvida_actual - DANHO_RAYO;
        printf("Vida reducida a %d\n", personaje->subvida_actual);
    }
}

