#include "characters.h"

/**
 * @brief Función que devuelve el tipo de frames de un cierto tipo de personaje.
 * @param tipo Es el tipo de personaje al cual se le desea obtener los tipos de frames correspondientes.
 * @return El tipo de frame del personaje.
 */
TipoFrame tipo_frame(TipoPersonaje tipo)
{
    switch (tipo)
    {
        case WOOFSON:
            return FRAME_WOOFSON;

        case DRAGON:
            return FRAME_DRAGON;
        
        case EXTRATERRESTRE:
            return FRAME_EXTRATERRESTRE;
        
        case MONSTRUO:
            return FRAME_MONSTRUO;

        case LEPRECHAUN:
            return FRAME_LEPRECHAUN;

        default:
            return FRAME_WOOFSON;
    }
}


/**
 * @brief Función que devuelve el tipo de personaje según el tipo de frames que tenga.
 * @param tipo Es el tipo de frame que tiene el personaje.
 * @return El tipo de personaje que corresponde con ese tipo de frames.
 */
TipoPersonaje tipo_personaje(TipoFrame tipo)
{
    switch (tipo)
    {
        case FRAME_WOOFSON:
            return WOOFSON;

        case FRAME_DRAGON:
            return DRAGON;

        case FRAME_EXTRATERRESTRE:
            return EXTRATERRESTRE;
        
        case FRAME_MONSTRUO:
            return MONSTRUO;
        
        case FRAME_LEPRECHAUN:
            return LEPRECHAUN;
    
        default:
            return WOOFSON;
    }
}


/**
 * @brief Función que inicializa un personaje según su tipo.
 * @param personaje Puntero al personaje a inicializar.
 * @param tipo Tipo de personaje a inicializar.
 * @param frames Es el arreglo con los frames de todos los personajes.
 * @param posicion_deseada Posición en la que se desea inicializar al personaje.
 * @param estatico Un booleano que determina si el personaje estará en una posición fija o no.
 */
Procedure inicializar_personaje(Personaje* personaje, TipoPersonaje tipo, Imagen* frames[TIPOS_PERSONAJES], Vector posicion_deseada, bool estatico)
{
    Natural i;

    personaje->tipo = tipo;

    if ((personaje->tipo == WOOFSON || personaje->tipo == DRAGON || personaje->tipo == MONSTRUO) && estatico == true)
    {
        return;  // No tiene sentido que se cargue el personaje principal y que no se pueda mover.  Así mismo, los dragones (ya que volarán) y el monstruo (que será duro).
    }

    personaje->posicion = posicion_deseada;
    personaje->estatico = estatico;

    if (personaje->estatico)
    {
        personaje->velocidad = VECTOR_NULO;
    }

    personaje->caminata = tipo!=WOOFSON && !personaje->estatico;  // Si el tipo de personaje es Woofson y/o el personaje es estático entonces parte quieto (sin caminar)
    personaje->victoria = tipo==WOOFSON ? false : true;  // Se inicializa como que los enemigos estan ganando y Woofson perdiendo
    personaje->fps_en_caminata = 0;
    personaje->fps_en_ataque = 0;
    personaje->frames = frames[tipo_frame(tipo)];
    personaje->imagen = personaje->frames[0];
    personaje->id_nro_frame = 0;
    personaje->ancho = al_get_bitmap_width(personaje->imagen);
    personaje->alto = al_get_bitmap_height(personaje->imagen);
    personaje->muerto = false;
    personaje->tiempo_muerte = 0;
    personaje->frames_para_prox_disparo = 0;
    personaje->subvida_actual = 100;  // Todos tendrán subvida de 100 inicialmente
    personaje->fuego = (Fuego) {0};

    switch (tipo)
    {
        case WOOFSON:
            personaje->posicion_inicial = posicion_deseada;
            personaje->en_ataque = false;
            personaje->velocidad = VECTOR_NULO;  // Inicialmente no se mueve ni salta, ya que eso lo deberá hacer con las teclas
            personaje->nro_vidas = VIDAS_INICIALES;
            personaje->bandera_dibujo = 0;  // 0: normal, ALLEGRO_FLIP_HORIZONTAL: espejo
            personaje->en_plataforma = false;  // Inicialmente no está en una plataforma (está en el suelo)
            personaje->danhado = true;  // Para que parta con cierta inmunidad antes de comenzar (parpadeo)
            personaje->tiempo_danho = 0;
            personaje->direccion = 1;
            personaje->bandera_dibujo = 0;
            inicializar_salto(personaje);  // Inicializa la estructura del salto para el personaje
            break;

        case DRAGON:
            personaje->velocidad.x = VELOCIDAD_DRAGONES;
            personaje->velocidad.y = 0;  // Inicialmente no está en salto
            personaje->en_ataque = true;
            personaje->fps_en_ataque = 0;
            personaje->frames_para_prox_disparo = 0;
            personaje->nro_vidas = 1;
            personaje->en_plataforma = false;
            personaje->danhado = false;
            personaje->tiempo_danho = 0;
            personaje->ancho *= 0.7;
            personaje->alto *= 0.7;
            // EN EL CASO DEL DRAGÓN, COMO VOLARÁ, NO SERÁ NECESARIO INICIALIZAR EL SALTO
            break;

        case EXTRATERRESTRE:
            personaje->velocidad.x = personaje->estatico ? 0 : VELOCIDAD_EXTRATERRESTRES;  // Esta velocidad vale solo si el extraterrestre es dinámico (no estático)
            personaje->velocidad.y = 0;  // Inicialmente no está en salto
            personaje->direccion = pow(-1, rand() % 2);
            personaje->nro_vidas = 1;  // Tendrá una sola vida
            personaje->subvida_actual = 100;
            personaje->bandera_dibujo = 0;  // 0: normal, ALLEGRO_FLIP_HORIZONTAL: espejo
            personaje->en_plataforma = false;  // Inicialmente no está en una plataforma (está en el suelo)
            personaje->danhado = false;
            personaje->tiempo_danho = 0;
            inicializar_salto(personaje);  // Inicializa la estructura del salto para el personaje
            break;

        case MONSTRUO:
            personaje->velocidad.x = VELOCIDAD_MONSTRUO;
            personaje->velocidad.y = 0;  // Inicialmente no está en salto
            break;
        
        case LEPRECHAUN:
            personaje->velocidad.x = VELOCIDAD_LEPRECHAUN;
            personaje->velocidad.y = 0;
            personaje->direccion = pow(-1, rand() % 2);
            personaje->nro_vidas = 1;  // Tendrá una sola vida
            personaje->subvida_actual = 100;
            personaje->bandera_dibujo = 0;  // 0: normal, ALLEGRO_FLIP_HORIZONTAL: espejo
            personaje->en_plataforma = false;  // Inicialmente no está en una plataforma (está en el suelo)
            personaje->danhado = false;
            personaje->tiempo_danho = 0;
            inicializar_salto(personaje);  // Inicializa la estructura del salto para el personaje
            break;
    }

    for (i=0; i<MAX_BALAS; i++)
    {
        personaje->balas[i] = (Bala) {0};
        personaje->balas[i].disponible = true;
        personaje->bala_recargable = personaje->tipo == WOOFSON ? false : true;
        personaje->bala_maximo_alcance = personaje->tipo == WOOFSON ? false : true;
        personaje->balas[i].frames_para_disponibilidad = 0;
    }

    if (personaje->tipo == WOOFSON)
    {
        personaje->barra_vida = (BarraVida) {0};  // Woofson tendrá otro tipo de barra de vida que se manejará directamente desde la pantalla
    }

    else
    {
        personaje->barra_vida.contenedor = obtener_rectangulo(RECTANGULO_VENTANA, 10, 50, (personaje->posicion.x + personaje->ancho/2) / ANCHO_VENTANA * 100, (personaje->posicion.y - 10) / ALTO_VENTANA * 100);
    }

    personaje->inicializado = true;
}


/**
 * @brief Función que inicializa la estructura de salto del personaje.
 * @param personaje Puntero al personaje cuya estructura de salto se va a inicializar.
 */
Procedure inicializar_salto(Personaje* personaje)
{
    personaje->salto.en_salto = false;
    personaje->salto.es_interrumpido = false;  // Indica si el salto es interrumpido por una colisión con algún techo o no
    personaje->salto.tiempo_en_salto = 0;  // Reinicia el tiempo de salto
    personaje->salto.impulso = IMPULSO_PERSONAJE;  // Si bien no es necesario, es una buena práctica para la legibilidad
    personaje->salto.altura_inicial = personaje->posicion.y;  // Reinicia la altura inicial del salto
    personaje->salto.altura_choque = 0;
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
        personaje->direccion = -1;
    }

    else
    {
        personaje->bandera_dibujo = 0;  // Dibuja el personaje mirando a la derecha
        personaje->direccion = 1;
    }
}


/**
 * Función que dibuja el personaje en su posición actual.
 * @param personaje El personaje a dibujar.
 * @param ultima_tecla_lateral La última tecla lateral presionada (para determinar la dirección de dibujo).
 * @param iteracion La iteración actual del loop general del juego.
 */
Procedure dibujar_personaje(Personaje* personaje, Natural ultima_tecla_lateral, Natural iteracion)
{
    Natural var;

    if (!personaje->inicializado)
    {
        return;
    }
    
    if (personaje->tipo == WOOFSON)
    {
        determinar_como_dibujar_personaje(personaje, ultima_tecla_lateral);
    }

    if (personaje->danhado && personaje->tiempo_danho > 0)
    {
        var = iteracion / (FPS/6);

        if (var & 1)
        {
            al_draw_scaled_bitmap(personaje->imagen, 0, 0, al_get_bitmap_width(personaje->imagen), al_get_bitmap_height(personaje->imagen), 
                                  personaje->posicion.x, personaje->posicion.y, personaje->ancho, personaje->alto, personaje->bandera_dibujo);
        }
    }

    else
    {
        al_draw_scaled_bitmap(personaje->imagen, 0, 0, al_get_bitmap_width(personaje->imagen), al_get_bitmap_height(personaje->imagen), 
                              personaje->posicion.x, personaje->posicion.y, personaje->ancho, personaje->alto, personaje->bandera_dibujo);
    }
}


/**
 * Función que actualiza el frame del personaje según su velocidad y estado de caminata.
 * @param personaje Puntero al personaje cuyo frame se va a actualizar.
 * @param modo Modo de actualización del frame (este parámetro tiene importancia solo si los frames que se actualizan son de Woofson)
 */
Procedure actualizar_frame(Personaje* personaje, ModoWoofson modo)
{
    if (personaje->estatico)
    {
        return;  // Si es estático no tiene sentido que sus frames se actualicen
    }

    if (personaje->tipo == WOOFSON)
    {
        if (modo == PELEA)  // Si está en modo pelea, se actualiza el frame de pelea
        {
            personaje->id_nro_frame = NRO_FRAMES_MOVIMIENTO + (personaje->id_nro_frame + 1) % NRO_FRAMES_PELEA ;  // Evita que el frame se salga del rango
        }

        else if (modo == DISPARO)
        {
            personaje->id_nro_frame = NRO_FRAMES_MOVIMIENTO + NRO_FRAMES_PELEA + (personaje->id_nro_frame + 1) % NRO_FRAMES_DISPARO;
        }

        else  // Si está en modo movimiento, se actualiza el frame de movimiento siempre que esté en movimiento o en pataleo
        {
            if (personaje->velocidad.x != 0)
            {
                personaje->id_nro_frame = (personaje->id_nro_frame + 1) % NRO_FRAMES_MOVIMIENTO;  // Evita que el frame se salga del rango
            }
        }
    }

    else if (personaje->tipo == EXTRATERRESTRE)
    {
        personaje->id_nro_frame = (personaje->id_nro_frame + 1) % NRO_FRAMES_EXTRATERRESTRE;
    }

    else if (personaje->tipo == DRAGON)
    {
        personaje->id_nro_frame = (personaje->id_nro_frame + 1) % NRO_FRAMES_DRAGON;
    }

    else if (personaje->tipo == LEPRECHAUN)
    {
        if (modo == PELEA)
        {
            personaje->id_nro_frame = NRO_FRAMES_MOVIMIENTO + (personaje->id_nro_frame + 1) % NRO_FRAMES_PELEA;
        }

        else
        {
            personaje->id_nro_frame = (personaje->id_nro_frame + 1) % NRO_FRAMES_MOVIMIENTO;
        }
    }

    if (personaje->frames[personaje->id_nro_frame])  // Verifica si hay un frame siguiente para evitar errores de acceso a memoria
    {
        personaje->imagen = personaje->frames[personaje->id_nro_frame];  // Cambia al siguiente frame de animación

        personaje->alto = al_get_bitmap_height(personaje->imagen) * (personaje->tipo == DRAGON ? 0.7 : 1);  // Actualiza el alto del personaje
        personaje->ancho = al_get_bitmap_width(personaje->imagen) * (personaje->tipo == DRAGON ? 0.7 : 1);  // Actualiza el ancho del personaje
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
    if (teclas[ALLEGRO_KEY_LEFT] && !hay_colision_izquierda(personaje, mapa)) //&& !personaje->hay_obj_izq)
    {
        return true;  // Puede moverse a la izquierda
    }

    else if (teclas[ALLEGRO_KEY_RIGHT] && !hay_colision_derecha(personaje, mapa)) //&& !personaje->hay_obj_der)
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
Procedure mover_personaje(Personaje* personaje, Mapa mapa, Natural nivel)
{
    ModoWoofson modo_ataque;

    if (personaje->muerto || personaje->victoria || (personaje->nro_vidas == 0 && personaje->subvida_actual == 0))  // En caso de que el personaje esté muerto, o en caso de victoria, o en caso de que perdió, no se podrá mover
    {
        return;
    }

    else  
    {
        personaje->caminata = teclas[ALLEGRO_KEY_LEFT] ^ teclas[ALLEGRO_KEY_RIGHT];  // Actualiza el estado de caminata según las teclas presionadas
    
        if (teclas[ALLEGRO_KEY_SPACE])
        {
            personaje->en_ataque = true;
            personaje->fps_en_ataque++;

            if (personaje->fps_en_ataque % 8 == 0)  // Actualiza el frame de pelea cada 8 frames
            {
                modo_ataque = nivel <= NIVEL2 ? PELEA : DISPARO;
                actualizar_frame(personaje, modo_ataque);  // Actualiza el frame del personaje si está en pelea
            }
        }

        else
        {
            personaje->ancho = al_get_bitmap_width(personaje->imagen);
            personaje->alto = al_get_bitmap_height(personaje->imagen);
            personaje->en_ataque = false;
        }
    
        if (personaje->caminata)
        {
            personaje->fps_en_caminata++;

            if (!teclas[ALLEGRO_KEY_SPACE])  // Si no está en pelea, actualiza el frame de caminata
            {
                if (personaje->velocidad.x != 0 && personaje->fps_en_caminata % (int) fabs((30 / ceilf(personaje->velocidad.x))) == 0)  // Actualiza el frame cada cierto numero de iteraciones que depende de la velocidad
                {
                    actualizar_frame(personaje, CAMINATA);  // Actualiza el frame del personaje si está caminando
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
                    actualizar_frame(personaje, CAMINATA);  // Actualiza el frame del personaje si está caminando
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
}


Procedure mover_enemigo_dinamico(Personaje* enemigo, Personaje woofson, Mapa mapa)
{
    ModoWoofson modo_leprechaun;
    float amplitud_dragon = 2;   

    if (enemigo->estatico || enemigo->muerto)  // No tiene sentido mover un enemigo estático o que ya murió
    {
        return;
    }
    
    if ((enemigo->direccion == -1 && hay_colision_izquierda(enemigo, mapa)) || (enemigo->direccion == 1 && hay_colision_derecha(enemigo, mapa)))
    {
        enemigo->direccion *= -1;
    }

    enemigo->posicion.x += enemigo->velocidad.x * enemigo->direccion;
    
    if (enemigo->tipo == DRAGON)
    {
        if (!hay_colision_superior(enemigo, mapa) && !hay_colision_inferior(enemigo, mapa))
        {
            enemigo->posicion.y += amplitud_dragon * sin((float) enemigo->fps_en_caminata / 12);
        }
    }

    if (enemigo->tipo == LEPRECHAUN)
    {
        if (enemigo->fps_en_caminata % 12 == 0)
        {
            modo_leprechaun = puede_disparar_horizontalmente(*enemigo, woofson, mapa) ? PELEA : CAMINATA;
            actualizar_frame(enemigo, modo_leprechaun);
        }
    }

    else
    {
        if (enemigo->fps_en_caminata % 8 == 0)
        {
            actualizar_frame(enemigo, '\0');  // El modo es '\0' ya que da igual para los enemigos
        }
    }

    if (enemigo->tipo != DRAGON)
    {
        // Si no está en salto, activa caída libre
        if (!enemigo->salto.en_salto && !hay_colision_inferior(enemigo, mapa))
        {
            activar_caida_libre(enemigo);
        }

        // Si está en salto (o caída), continuar con física
        if (enemigo->salto.en_salto)
        {
            enemigo->salto.tiempo_en_salto += 1./FPS;
            continuar_salto(enemigo, enemigo->salto.tiempo_en_salto, mapa);
        }
    }

    enemigo->fps_en_caminata++;
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
 * @param t El tiempo actual.
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


Procedure morir(Personaje* personaje, Tecla* ultima_lateral, Etapa* etapa_actual)
{
    Imagen bitmap = personaje->imagen_modo_muerte;
    float ancho = al_get_bitmap_width(personaje->imagen_modo_muerte);
    float alto = al_get_bitmap_height(personaje->imagen_modo_muerte);
    float factor_escala, dx, dy;

    factor_escala = 1.5;

    if (personaje->victoria)
    {
        return;
    }

    if (personaje->muerto)
    {
        personaje->tiempo_muerte += 1./FPS;

        dx = personaje->posicion.x + personaje->ancho / 2 - ancho*factor_escala / 2;
        dy = personaje->posicion.y + personaje->alto / 2 - alto*factor_escala / 2;

        al_draw_scaled_bitmap(bitmap, 0, 0, ancho, alto, dx, dy, ancho*factor_escala, alto*factor_escala, 0);

        if (personaje->tiempo_muerte >= TIEMPO_MUERTE)
        {
            if (*etapa_actual == DERROTA)
            {
                dx = personaje->posicion.x + personaje->ancho / 2 - ancho*factor_escala / 2;
                dy = personaje->posicion.y + personaje->alto / 2 - alto*factor_escala / 2;
                al_draw_scaled_bitmap(bitmap, 0, 0, ancho, alto, dx, dy, ancho*factor_escala, alto*factor_escala, 0);
            }

            if (personaje->nro_vidas == 0)
            {
                *etapa_actual = DERROTA;
                return;
            }

            personaje->nro_vidas--;
            personaje->subvida_actual = 100;
            personaje->muerto = false;
            personaje->tiempo_muerte = 0;
            personaje->imagen = personaje->frames[0];
            personaje->id_nro_frame = 0;  // Se resetea el número de frame actual a 0
            personaje->posicion.x = ANCHO_VENTANA * 0.1;  // Se restablece su posición inicial
            personaje->posicion.y = ALTURA_PISO - personaje->alto;  // Se coloca en el piso
            personaje->bandera_dibujo = 0;  // 0: normal, ALLEGRO_FLIP_HORIZONTAL: espejo
            personaje->en_plataforma = false;  
            personaje->danhado = true;  // Para que parta con cierto tiempo de inmunidad luego de morir (en caso de que haya un enemigo demasiado cerca por ejemplo)
            personaje->tiempo_danho = 0;
            personaje->velocidad.x = 0;
            personaje->velocidad.y = 0;  
            personaje->caminata = false;
            personaje->fps_en_caminata = 0; 
            inicializar_salto(personaje);
            *ultima_lateral = ALLEGRO_KEY_RIGHT;  // Para garantizar que el personaje además de partir en la posición deseada (la inicial), parta mirando hacia la derecha
        }
    }
}


/**
 * @brief Procedimiento que determina si Woofson gana, y si es así, deja la victoria de Woofson en true, y la de los enemigos en false.
 * @param personaje Puntero a Woofson.
 * @param enemigo Arreglo con los enemigos (que se rendirán si Woofson gana).
 * @param puerta Es la puerta de paso de nivel.
 * @param etapa_actual Es un puntero a la etapa actual del juego.
 */
Procedure determinar_victoria_woofson(Personaje* personaje, Personaje enemigos[MAX_ENEMIGOS], Puerta puerta, Etapa* etapa_actual)
{
    Natural i;
    Natural nro_enemigos = nro_enemigos_activos(enemigos);

    if (puerta.estado == ABIERTA)
    {
        if (personaje->posicion.x + personaje->ancho >= puerta.posicion.x + puerta.ancho / 2 && personaje->posicion.x < puerta.posicion.x + puerta.ancho / 2 &&
            personaje->posicion.y + personaje->alto >= puerta.posicion.y && personaje->posicion.y < puerta.posicion.y + puerta.alto)
        {
            personaje->victoria = true;
            
            for (i=0; i<nro_enemigos; i++)
            {
                enemigos[i].victoria = false;  // Se rinden los enemigos
            }

            *etapa_actual = VICTORIA;
        }
    }
}


Procedure aplicar_danho(Personaje* personaje, Natural cantidad_danho)
{
    if (personaje->subvida_actual <= cantidad_danho)
    {
        personaje->subvida_actual = 0;
        personaje->muerto = true;
    }

    else
    {
        personaje->subvida_actual -= cantidad_danho;
    }
}


Procedure aumentar_subvida(Personaje* personaje, Natural cantidad_aumento)
{
    if (cantidad_aumento >= 100 - personaje->subvida_actual)
    {
        personaje->subvida_actual = 100;
    }

    else
    {
        personaje->subvida_actual += cantidad_aumento;
    }
}


Procedure actualizar_estado_danho(Personaje* personaje)
{
    if (personaje->danhado) 
    {
        if (personaje->tiempo_danho <= (personaje->tipo == WOOFSON ? MAX_TIEMPO_INMUNE_WOOFSON : MAX_TIEMPO_INMUNE_ENEMIGO))
        {
            personaje->tiempo_danho += 1.0 / FPS;
        } 
        
        else 
        {
            personaje->danhado = false;
            personaje->tiempo_danho = 0;
        }
    }

    if (personaje->tipo != WOOFSON)
    {
        if (personaje->muerto)
        {
            if (personaje->tiempo_muerte <= TIEMPO_MUERTE_ENEMIGO)
            {
                personaje->posicion = VECTOR_NULO;
                personaje->tiempo_muerte += 1.0 / FPS;
            }

            else
            {
                personaje->posicion = personaje->posicion_inicial;
                personaje->muerto = false;
                personaje->subvida_actual = 100;
                personaje->tiempo_muerte = 0;
            }
        }

        else
        {
            personaje->barra_vida.contenedor = dibujar_rectangulo_en_rectangulo(RECTANGULO_VENTANA, 10, 50, (personaje->posicion.x + personaje->ancho/2) / ANCHO_VENTANA * 100, 
                                                                               (personaje->posicion.y - 10) / ALTO_VENTANA * 100, false, NEGRO);

            personaje->barra_vida.subvida = dibujar_rectangulo_en_rectangulo(personaje->barra_vida.contenedor, 8, 48 * (float) personaje->subvida_actual / 100, 
                                                                             50.0 * (float) personaje->subvida_actual / 100, 50.0, true, NARANJO);
        }
    }
}


Procedure actualizar_estado_danho_enemigos(Personaje enemigos[MAX_ENEMIGOS])
{
    Natural i, cantidad_enemigos;

    cantidad_enemigos = nro_enemigos_activos(enemigos);

    for (i=0; i<cantidad_enemigos; i++)
    {
        actualizar_estado_danho(&enemigos[i]);
    }
}


Procedure detectar_si_personaje_en_zona_de_rayo(Personaje* personaje, Rayo rayo[MAX_RAYOS])
{
    Natural i;

    if (personaje->muerto || personaje->danhado)   // Si el personaje ya está muerto o dañado no se hace nada
    {
        return;
    }

    for (i=0; i<MAX_RAYOS; i++)
    {
        if (rayo[i].activo && !personaje->danhado)
        {
            if (rayo[i].origen.x == rayo[i].objetivo.x)  // Rayo vertical
            {
                if (personaje->posicion.y < rayo[i].posicion.y && 
                    personaje->posicion.y + personaje->alto < rayo[i].objetivo.y &&
                    personaje->posicion.x + personaje->ancho > rayo[i].posicion.x - rayo[i].grosor/2 &&
                    personaje->posicion.x < rayo[i].posicion.x + rayo[i].grosor/2)
                {
                    if (!personaje->danhado)
                    {
                        personaje->danhado = true;
                        personaje->tiempo_danho = 0;
                    }

                    break;
                }
            }

            else  // Rayo horizontal
            {
                if (personaje->posicion.x < rayo[i].posicion.x &&
                    personaje->posicion.x + personaje->ancho < rayo[i].objetivo.x &&
                    personaje->posicion.y + personaje->alto > rayo[i].posicion.y - rayo[i].grosor/2 && 
                    personaje->posicion.y < rayo[i].posicion.y + rayo[i].grosor/2)
                {
                    personaje->danhado = true;
                    personaje->tiempo_danho = 0;
                    break;
                }
            }
        }
    }
    
    if (personaje->danhado)  // Si el daño causado por el rayo fue reciente, entonces se le quita vida
    {
        if (personaje->tiempo_danho == 0)
        {
            aplicar_danho(personaje, DANHO_RAYO);
        }
    }
}


bool hay_balas_activas(Bala balas[MAX_BALAS])
{
    Natural i;

    for (i=0; i<MAX_BALAS; i++)
    {
        if (balas[i].activa)
        {
            return true;
        }
    }

    return false;
}


Natural nro_balas_disponibles(Bala balas[MAX_BALAS])
{
    Natural i, disponibles = 0;

    for (i=0; i<MAX_BALAS; i++)
    {
        if (balas[i].disponible)
        {
            disponibles++;
        }
    }

    return disponibles;
}


Procedure mover_balas_activas(Personaje* atacante, Personaje* victima, Mapa mapa, Natural nivel, ALLEGRO_COLOR color)
{
    Natural i;
    int fil, col;

    for (i=0; i<MAX_BALAS; i++)
    {
        if (atacante->balas[i].activa)
        {
            atacante->balas[i].posicion.x += atacante->balas[i].velocidad.x;  // Si Woofson gana, las balas se detienen (a modo como de congelar el juego)

            if (atacante->bala_maximo_alcance == true)
            {
                if (atacante->balas[i].posicion.x - RADIO_AXIAL_X_BALA > ANCHO_VENTANA || atacante->balas[i].posicion.x + RADIO_AXIAL_X_BALA < 0)
                {
                    atacante->balas[i].direccion = 0;  
                    atacante->balas[i].velocidad = VECTOR_NULO;  // Se deja de mover
                    atacante->balas[i].activa = false;  // Se desactiva la bala
                    continue;
                }
            }

            else
            {
                if (nivel >= 4)
                {
                    if (atacante->bala_maximo_alcance)
                    {
                        if (atacante->balas[i].posicion.x - RADIO_AXIAL_X_BALA_PRO > ANCHO_VENTANA || atacante->balas[i].posicion.x + RADIO_AXIAL_X_BALA_PRO < 0)
                        {
                            atacante->balas[i].direccion = 0;  
                            atacante->balas[i].velocidad = VECTOR_NULO;  // Se deja de mover
                            atacante->balas[i].activa = false;  // Se desactiva la bala
                            continue;
                        }
                    }

                    else
                    {
                        if (fabs(atacante->balas[i].posicion.x - atacante->balas[i].posicion_inicial.x) > RANGO_BALA_PRO_WOOFSON)
                        {
                            atacante->balas[i].direccion = 0;  
                            atacante->balas[i].velocidad = VECTOR_NULO;  // Se deja de mover
                            atacante->balas[i].activa = false;  // Se desactiva la bala
                            continue;
                        }
                    }
                }

                else
                {
                    if (atacante->bala_maximo_alcance)
                    {
                        if (atacante->balas[i].posicion.x - RADIO_AXIAL_X_BALA > ANCHO_VENTANA || atacante->balas[i].posicion.x + RADIO_AXIAL_X_BALA < 0)
                        {
                            atacante->balas[i].direccion = 0;  
                            atacante->balas[i].velocidad = VECTOR_NULO;  // Se deja de mover
                            atacante->balas[i].activa = false;  // Se desactiva la bala
                            continue;
                        }
                    }

                    else
                    {
                        if (fabs(atacante->balas[i].posicion.x - atacante->balas[i].posicion_inicial.x) > RANGO_BALA_WOOFSON)
                        {
                            atacante->balas[i].direccion = 0;  
                            atacante->balas[i].velocidad = VECTOR_NULO;  // Se deja de mover
                            atacante->balas[i].activa = false;  // Se desactiva la bala
                            continue;
                        }
                    }
                }
            }

            // Verificar colisión con Woofson   
            if (!victima->danhado && (victima->tipo != WOOFSON || (victima->tipo == WOOFSON && !victima->victoria)))
            {
                if (atacante->balas[i].posicion.x + RADIO_AXIAL_X_BALA > victima->posicion.x &&
                    atacante->balas[i].posicion.x - RADIO_AXIAL_X_BALA < victima->posicion.x + victima->ancho &&
                    atacante->balas[i].posicion.y + RADIO_AXIAL_Y_BALA > victima->posicion.y &&
                    atacante->balas[i].posicion.y - RADIO_AXIAL_Y_BALA < victima->posicion.y + victima->alto)
                {
                    atacante->balas[i].direccion = 0;
                    atacante->balas[i].velocidad = VECTOR_NULO;
                    atacante->balas[i].posicion = VECTOR_NULO;

                    victima->danhado = true;
                    victima->tiempo_danho = 0;

                    if (atacante->tipo == WOOFSON)
                    {
                        if (nivel >= 4)
                        {
                            if (victima->tipo == EXTRATERRESTRE)
                            {
                                aplicar_danho(victima, 86);
                            }

                            else if (victima->tipo == DRAGON)
                            {
                                aplicar_danho(victima, 45);
                            }

                            else  // MONSTRUO
                            {
                                aplicar_danho(victima, 13);
                            }
                        }

                        else
                        {
                            if (victima->tipo == EXTRATERRESTRE)
                            {
                                aplicar_danho(victima, 34);
                            }

                            else if (victima->tipo == DRAGON)
                            {
                                aplicar_danho(victima, 17);
                            }

                            else  // MONSTRUO
                            {
                                aplicar_danho(victima, 5);
                            }
                        }
                    }

                    else
                    {
                        aplicar_danho(victima, DANHO_BALA_EXTRATERRESTRE);
                    }

                    if (atacante->tipo == WOOFSON)
                    {
                        if (victima->subvida_actual == 0)
                        {
                            victima->muerto = true;
                            victima->tiempo_muerte = 0;

                            if (victima->tipo == EXTRATERRESTRE)  
                            {
                                puntuacion += 10;  // Se dan 10 puntos por matar a un extraterrestre
                            }

                            else if (victima->tipo == DRAGON)
                            {
                                puntuacion += 25;  // Se dan 25 puntos por matar a un dragón
                            }

                            else
                            {
                                if (victima->tipo == MONSTRUO)
                                {
                                    puntuacion += 100;  // Se dan 100 puntos por matar al monstruo
                                }
                            }
                        }

                        else  // Si no se mata al enemigo
                        {
                            if (victima->tipo == EXTRATERRESTRE)  
                            {
                                puntuacion += 2;  // Se dan 2 puntos por cada daño hecho a un extraterrestre (sin matarlo)
                            }

                            else if (victima->tipo == DRAGON)
                            {
                                puntuacion += 7;  // Se dan 7 puntos por cada daño hecho a un dragón (sin matarlo)
                            }

                            else
                            {
                                if (victima->tipo == MONSTRUO)
                                {
                                    puntuacion += 15;  // Se dan 15 puntos por cada daño hecho al monstruo (sin matarlo)
                                }
                            }
                        }
                    }

                    atacante->balas[i].activa = false;

                    continue;
                }
            }

            fil = atacante->balas[i].posicion.y / mapa.alto_bloque;
            col = atacante->balas[i].posicion.x / mapa.ancho_bloque;

            if (X_ENTRE_A_Y_B(fil, 0, mapa.nro_filas) && X_ENTRE_A_Y_B(col, 0, mapa.nro_columnas) && (mapa.mapa[fil][col] == BLOQUE || mapa.mapa[fil][col] == BLOQUE_RAYO))
            {
                atacante->balas[i].direccion = 0;
                atacante->balas[i].velocidad = VECTOR_NULO;
                atacante->balas[i].posicion = VECTOR_NULO;
                atacante->balas[i].activa = false;
            }

            if (atacante->tipo != WOOFSON || nivel < 4)
            {
                al_draw_filled_ellipse(atacante->balas[i].posicion.x, atacante->balas[i].posicion.y, RADIO_AXIAL_X_BALA, RADIO_AXIAL_Y_BALA, color);
            }

            else
            {
                al_draw_filled_ellipse(atacante->balas[i].posicion.x, atacante->balas[i].posicion.y, RADIO_AXIAL_X_BALA_PRO, RADIO_AXIAL_Y_BALA_PRO, color);
            }
        }
    }
}


bool puede_disparar_horizontalmente(Personaje enemigo, Personaje woofson, Mapa mapa)
{
    Natural fila, col;
    Natural col_enemigo, col_woofson, col_min, col_max;
    bool alineados_en_y;
    bool mirando_a_woofson;

    if (enemigo.frames_para_prox_disparo > 0)
    {
        return false;
    }

    // Verificamos que estén alineados verticalmente (en el eje Y)
    alineados_en_y = woofson.posicion.y < enemigo.posicion.y + enemigo.alto && woofson.posicion.y + woofson.alto > enemigo.posicion.y;

    if (!alineados_en_y)
    {
        return false;
    }

    // Determinamos las columnas (en bloques) entre el enemigo y Woofson
    col_enemigo = (enemigo.posicion.x + enemigo.ancho / 2) / mapa.ancho_bloque;
    col_woofson = (woofson.posicion.x + woofson.ancho / 2) / mapa.ancho_bloque;
    fila = (enemigo.posicion.y + enemigo.alto / 2) / mapa.alto_bloque;

    col_min = fmin(col_enemigo, col_woofson);
    col_max = fmax(col_enemigo, col_woofson);

    // Verificar que no haya bloque en la misma fila, entre las columnas del enemigo y del jugador
    for (col=col_min+1; col<col_max; col++)
    {
        if (mapa.mapa[fila][col] == BLOQUE || mapa.mapa[fila][col] == BLOQUE_RAYO)
        {
            return false;  // Obstruido
        }
    }

    mirando_a_woofson = false;

    if (enemigo.bandera_dibujo == (enemigo.tipo == LEPRECHAUN ? 0 : ALLEGRO_FLIP_HORIZONTAL) && woofson.posicion.x + woofson.ancho <= enemigo.posicion.x)
    {
        mirando_a_woofson = true;
    }

    else if (enemigo.bandera_dibujo == (enemigo.tipo == LEPRECHAUN ? ALLEGRO_FLIP_HORIZONTAL : 0) && woofson.posicion.x >= enemigo.posicion.x + enemigo.ancho)
    {
        mirando_a_woofson = true;
    }

    else
    {
        mirando_a_woofson = false;
    }

    return mirando_a_woofson;  // Puede disparar
}


Procedure efectuar_disparo_de_enemigo(Personaje* enemigo, Personaje* woofson, Mapa mapa, Natural nivel)
{
    Natural i;
    float dx;
    
    if (hay_balas_activas(enemigo->balas))
    {
        mover_balas_activas(enemigo, woofson, mapa, nivel, nivel != 4 ? VERDE : AMARILLO);
    }
    
    if (!enemigo->inicializado || enemigo->muerto)
    {
        return;
    }
    
    if (enemigo->frames_para_prox_disparo > 0)
    {
        enemigo->frames_para_prox_disparo--;
    }
    
    if (puede_disparar_horizontalmente(*enemigo, *woofson, mapa))
    {
        for (i=0; i<MAX_BALAS; i++)  // Buscamos una bala disponible en el arreglo de balas del enemigo
        {
            if (!enemigo->balas[i].activa)//enemigo->balas[i].velocidad.x == 0 && enemigo->balas[i].velocidad.y == 0)
            {
                enemigo->balas[i].activa = true;

                // Inicializar posición y dirección de la bala
                enemigo->balas[i].posicion = (Vector) {enemigo->bandera_dibujo == 0 ? enemigo->posicion.x + enemigo->ancho : enemigo->posicion.x, 
                                                       enemigo->posicion.y + 0.45f * enemigo->alto};

                dx = woofson->posicion.x - enemigo->posicion.x;

                enemigo->balas[i].direccion = (dx >= 0) ? 1 : -1;
                enemigo->balas[i].velocidad.x = enemigo->balas[i].direccion * VELOCIDAD_BALA;
                enemigo->balas[i].velocidad.y = 0;
                enemigo->frames_para_prox_disparo = MAX_FRAMES_ESPERA_ENEMIGO;                
                // AQUI LUEGO PODRIA IR EFECTO DE SONIDO
                break;
            }
        }
    }
}

Procedure lanzar_fuego(Personaje* dragon, Personaje* woofson, Imagen imagen_fuego)
{      
    float porcentajes_boca[] = {18.0, 6.7, 20.0, 39.5, 44.4, 57.0, 45.0, 40.0, 20.8, 18.8};

    if (!dragon->en_ataque)
    {   
        dragon->frames_para_prox_disparo--;

        if (dragon->frames_para_prox_disparo == 0)
        {
            dragon->en_ataque = true;
            dragon->fps_en_ataque = 0;
        }

        else
        {
            return;
        }
    }

    if (dragon->fps_en_ataque >= 200)
    {
        dragon->en_ataque = false;
        dragon->fps_en_ataque = 0;
        dragon->frames_para_prox_disparo = MAX_FRAMES_ESPERA_DRAGON;
        return;
    }

    if (dragon->muerto || !dragon->inicializado)
    {
        return;
    }
    
    dragon->fuego.imagen = imagen_fuego;
    dragon->fuego.alto = al_get_bitmap_height(dragon->fuego.imagen);
    dragon->fuego.ancho = al_get_bitmap_width(dragon->fuego.imagen);
    dragon->fuego.porcentaje_progreso = dragon->fps_en_ataque <= 100 ? dragon->fps_en_ataque : 200 - dragon->fps_en_ataque;

    if (dragon->direccion == -1)
    {
        dragon->fuego.posicion.x = dragon->posicion.x - dragon->fuego.porcentaje_progreso / 100 * dragon->fuego.ancho;  // Si va hacia la izquierda
    }

    else
    {
        dragon->fuego.posicion.x = dragon->posicion.x + dragon->ancho;// + fuego.porcentaje_progreso / 100 * fuego.ancho;
    }
    
    dragon->fuego.posicion.y = dragon->posicion.y + porcentajes_boca[dragon->id_nro_frame] / 100 * dragon->alto;  // 0.2f ya que por ahi esta su boca

    
    al_draw_scaled_bitmap(dragon->fuego.imagen, dragon->bandera_dibujo == 0 ? 0 : dragon->fuego.ancho * (100 - dragon->fuego.porcentaje_progreso) / 100,
                          0, dragon->fuego.ancho * dragon->fuego.porcentaje_progreso / 100, dragon->fuego.alto, dragon->fuego.posicion.x,
                          dragon->fuego.posicion.y, dragon->fuego.ancho * dragon->fuego.porcentaje_progreso / 100,
                          dragon->fuego.alto * dragon->fuego.porcentaje_progreso / 100, dragon->bandera_dibujo == 0 ? 0 : ALLEGRO_FLIP_HORIZONTAL);

    if (dragon->direccion == 1)
    {
        if (woofson->posicion.x + woofson->ancho > dragon->fuego.posicion.x && woofson->posicion.x < dragon->fuego.posicion.x + dragon->fuego.ancho * dragon->fuego.porcentaje_progreso / 100 &&
            woofson->posicion.y + woofson->alto > dragon->fuego.posicion.y && woofson->posicion.y < dragon->fuego.posicion.y + dragon->fuego.alto * dragon->fuego.porcentaje_progreso / 100)
        {
            if (!woofson->danhado)
            {
                woofson->danhado = true;
                woofson->tiempo_danho = 0;
                aplicar_danho(woofson, DANHO_FUEGO);
            }
        }
    }

    else
    {
        if (woofson->posicion.x + woofson->ancho > dragon->fuego.posicion.x + dragon->fuego.ancho * (100 - dragon->fuego.porcentaje_progreso) / 100 && 
            woofson->posicion.x < dragon->fuego.posicion.x + dragon->fuego.ancho &&
            woofson->posicion.y + woofson->alto > dragon->fuego.posicion.y && woofson->posicion.y < dragon->fuego.posicion.y + dragon->fuego.alto * dragon->fuego.porcentaje_progreso / 100)
        {
            if (!woofson->danhado)
            {
                woofson->danhado = true;
                woofson->tiempo_danho = 0;
                aplicar_danho(woofson, DANHO_FUEGO);
            }
        }
    }
    
    dragon->fps_en_ataque++;
}

Procedure efectuar_disparo_de_enemigos(Personaje enemigos[MAX_ENEMIGOS], Personaje* woofson, Mapa mapa, Natural nivel, Imagen fuego)
{
    Natural i;

    for (i=0; i<MAX_ENEMIGOS; i++)
    {
        if (enemigos[i].inicializado)
        {
            if (enemigos[i].tipo == DRAGON)
            {
                lanzar_fuego(&enemigos[i], woofson, fuego);
            }

            else if (enemigos[i].tipo == LEPRECHAUN)
            {
                // NADA
            }

            else
            {
                efectuar_disparo_de_enemigo(&enemigos[i], woofson, mapa, nivel);
            }
        }
    }
}


Natural nro_enemigos_activos(Personaje enemigos[MAX_ENEMIGOS])
{
    Natural i, cantidad = 0;

    for (i=0; i<MAX_ENEMIGOS; i++)
    {
        if (enemigos[i].inicializado)
        {
            cantidad++;
        }
    }

    return cantidad;
}


bool woofson_puede_disparar(Personaje* woofson)
{
    Natural i;
    Natural id_frames_disparo[] = {23, 24, 25, 26, 27, 28, 29};
    Natural nro_frames_disparo;

    if (!woofson->inicializado || woofson->frames_para_prox_disparo != 0 || nro_balas_disponibles(woofson->balas) == 0 || !teclas[ALLEGRO_KEY_SPACE])
    {
        return false;
    }

    nro_frames_disparo = sizeof(id_frames_disparo) / sizeof(id_frames_disparo[0]);

    for (i=0; i<nro_frames_disparo; i++)
    {
        if (woofson->id_nro_frame == id_frames_disparo[i])
        {
            return true;
        }
    }

    return false;
}


Procedure efectuar_disparo_de_woofson(Personaje* woofson, Personaje enemigos[MAX_ENEMIGOS], Mapa mapa, Natural nivel)
{
    Natural i, nro_enemigos;
    
    nro_enemigos = nro_enemigos_activos(enemigos);

    if (woofson->bala_maximo_alcance)
    {
        woofson->tiempo_restante_bala_maximo_alcance -= 1./FPS;

        if (woofson->tiempo_restante_bala_maximo_alcance <= 0)
        {
            woofson->bala_maximo_alcance = false;
            woofson->tiempo_restante_bala_maximo_alcance = 0;
        }
    }
    
    if (!(woofson->muerto && woofson->subvida_actual == 0 && woofson->nro_vidas == 0))  // Si no ha perdido aún, se mueven las balas
    {
        if (hay_balas_activas(woofson->balas))
        {
            if (nro_enemigos == 0)
            {   
                mover_balas_activas(woofson, &enemigos[0], mapa, nivel, NARANJO);
            }

            else
            {        
                for (i=0; i<MAX_ENEMIGOS; i++)
                {
                    if (enemigos[i].inicializado)
                    {
                        mover_balas_activas(woofson, &enemigos[i], mapa, nivel, NARANJO);
                    }
                }
            }
        }
    }
    
    if (!woofson->inicializado || woofson->muerto)
    {
        return;
    }

    if (woofson->frames_para_prox_disparo > 0)
    {
        woofson->frames_para_prox_disparo--;
    }

    if (woofson->bala_recargable)
    {
        for (i=0; i<MAX_BALAS; i++)
        {
            if (woofson->balas[i].frames_para_disponibilidad > 0)
            {
                woofson->balas[i].frames_para_disponibilidad--;

                if (woofson->balas[i].frames_para_disponibilidad == 0)
                {
                    woofson->balas[i].disponible = true;
                }
            }
        }
    }
    
    if (woofson_puede_disparar(woofson))
    {
        for (i=0; i<MAX_BALAS; i++)  // Buscamos una bala disponible en el arreglo de balas del enemigo
        {
            if (woofson->balas[i].disponible) //enemigo->balas[i].velocidad.x == 0 && enemigo->balas[i].velocidad.y == 0)
            {
                woofson->balas[i].activa = true;
                woofson->balas[i].disponible = false;
                woofson->balas[i].frames_para_disponibilidad = MAX_FRAMES_ESPERA_RECARGA;

                // Inicializar posición y dirección de la bala
                woofson->balas[i].posicion = (Vector) {woofson->bandera_dibujo == 0 ? woofson->posicion.x + woofson->ancho : woofson->posicion.x, 
                                                       woofson->posicion.y + 0.45f * woofson->alto};

                woofson->balas[i].posicion_inicial = woofson->balas[i].posicion;
                woofson->balas[i].direccion = woofson->direccion;

                if (nro_enemigos == 0)
                {
                    woofson->balas[i].velocidad.x = woofson->balas[i].direccion * VELOCIDAD_BALA;
                }

                else
                {
                    woofson->balas[i].velocidad.x = woofson->balas[i].direccion * (float) VELOCIDAD_BALA / nro_enemigos;  // En Woofson se va a llamar la funcion hay_balas_activas tantas veces como nro de enemigos haya
                }

                woofson->balas[i].velocidad.y = 0;
                woofson->frames_para_prox_disparo = MAX_FRAMES_ESPERA_WOOFSON;                
                // AQUI LUEGO PODRIA IR EFECTO DE SONIDO
                break;
            }
        }
    }    
}


/**
 * @brief Función que desactiva los enemigos para que se puedan inicializar de nuevo para otro nivel.
 * @param enemigos El arreglo con los enemigos.
 */
Procedure desactivar_enemigos(Personaje enemigos[MAX_ENEMIGOS])
{
    Natural i;

    for (i=0; i<MAX_ENEMIGOS; i++)
    {
        enemigos[i].inicializado = false;
    }
}