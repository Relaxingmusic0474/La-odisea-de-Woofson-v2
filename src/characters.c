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
    
        default:
            return WOOFSON;
    }
}


/**
 * @brief Función que inicializa un personaje según su tipo.
 * @param personaje Puntero al personaje a inicializar.
 * @param tipo Tipo de personaje a inicializar.
 * @param frames Es el arreglo con los frames de todos los personajes.
 * @param estatico Un booleano que determina si el personaje estará en una posición fija o no.
 */
Procedure inicializar_personaje(Personaje* personaje, TipoPersonaje tipo, Imagen* frames[TIPOS_PERSONAJES], Vector posicion_deseada, bool estatico)
{
    if ((personaje->tipo == WOOFSON || personaje->tipo == DRAGON || personaje->tipo == MONSTRUO) && estatico == true)
    {
        return;  // No tiene sentido que se cargue el personaje principal y que no se pueda mover.  Así mismo, los dragones (ya que volarán) y el monstruo (que será duro).
    }

    personaje->tipo = tipo;
    personaje->posicion = posicion_deseada;
    personaje->estatico = estatico;

    if (personaje->estatico)
    {
        personaje->velocidad = VECTOR_NULO;
    }

    personaje->caminata = tipo!=WOOFSON && !personaje->estatico;  // Si el tipo de personaje es Woofson y/o el personaje es estático entonces parte quieto (sin caminar)
    personaje->fps_en_caminata = 0;
    personaje->frames = frames[tipo_frame(tipo)];
    personaje->imagen = personaje->frames[0];
    personaje->id_nro_frame = 0;
    personaje->ancho = al_get_bitmap_width(personaje->imagen);
    personaje->alto = al_get_bitmap_height(personaje->imagen);
    personaje->muerto = false;
    personaje->tiempo_muerte = 0;
    personaje->frames_para_prox_disparo = 0;

    switch (tipo)
    {
        case WOOFSON:
            personaje->velocidad = VECTOR_NULO;  // Inicialmente no se mueve ni salta, ya que eso lo deberá hacer con las teclas
            personaje->nro_vidas = VIDAS_INICIALES;
            personaje->subvida_actual = 100;
            personaje->bandera_dibujo = 0;  // 0: normal, ALLEGRO_FLIP_HORIZONTAL: espejo
            personaje->en_plataforma = false;  // Inicialmente no está en una plataforma (está en el suelo)
            personaje->danhado = true;  // Para que parta con cierta inmunidad antes de comenzar (parpadeo)
            personaje->tiempo_danho = 0;
            inicializar_salto(personaje);  // Inicializa la estructura del salto para el personaje
            break;

        case DRAGON:
            personaje->velocidad.x = VELOCIDAD_DRAGONES;
            personaje->velocidad.y = 0;  // Inicialmente no está en salto

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
    }

    personaje->inicializado = true;
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
        personaje->direccion = -1;  // Significa que su dirección de movimiento es hacia la izquierda
    }

    else
    {
        personaje->bandera_dibujo = 0;  // Dibuja el personaje mirando a la derecha
    }
}


/**
 * Función que dibuja el personaje en su posición actual.
 * @param personaje El personaje a dibujar.
 * @param ultima_tecla_lateral La última tecla lateral presionada (para determinar la dirección de dibujo).
 * @param iteracion La iteración actual del loop general del juego.
 */
Procedure dibujar_personaje(Personaje personaje, Natural ultima_tecla_lateral, Natural iteracion)
{
    extern bool teclas[ALLEGRO_KEY_MAX];  // Arreglo global de teclas presionadas
    Natural var;
    
    if (personaje.tipo == WOOFSON)
    {
        determinar_como_dibujar_personaje(&personaje, ultima_tecla_lateral);
    }

    if (personaje.danhado && personaje.tiempo_danho > 0)
    {
        var = iteracion / (FPS/6);

        if (var & 1)
        {
            al_draw_bitmap(personaje.imagen, personaje.posicion.x, personaje.posicion.y, personaje.bandera_dibujo);
        }
    }

    else
    {
        al_draw_bitmap(personaje.imagen, personaje.posicion.x, personaje.posicion.y, personaje.bandera_dibujo);   
    }
}


/**
 * Función que actualiza el frame del personaje según su velocidad y estado de caminata.
 * @param personaje Puntero al personaje cuyo frame se va a actualizar.
 * @param modo Modo de actualización del frame ('M' para movimiento, 'P' para pelea).
 */
Procedure actualizar_frame(Personaje* personaje, char modo)
{
    if (personaje->estatico)
    {
        return;  // Si es estático no tiene sentido que sus frames se actualicen
    }

    if (personaje->tipo == WOOFSON)
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
    }

    else if (personaje->tipo == EXTRATERRESTRE)
    {
        personaje->id_nro_frame = (personaje->id_nro_frame + 1) % NRO_FRAMES_EXTRATERRESTRE;
    }

    if (personaje->frames[personaje->id_nro_frame])  // Verifica si hay un frame siguiente para evitar errores de acceso a memoria
    {
        personaje->imagen = personaje->frames[personaje->id_nro_frame];  // Cambia al siguiente frame de animación
        personaje->alto = al_get_bitmap_height(personaje->imagen);  // Actualiza el alto del personaje
        personaje->ancho = al_get_bitmap_width(personaje->imagen);  // Actualiza el ancho del personaje
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
Procedure mover_personaje(Personaje* personaje, Mapa mapa)
{
    extern bool teclas[ALLEGRO_KEY_MAX];  // Arreglo global de teclas presionadas

    if (personaje->muerto)  // En caso de que el personaje esté muerto, no se podrá mover
    {
        return;
    }

    else  
    {
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
}


Procedure mover_enemigo_dinamico(Personaje* enemigo, Mapa mapa)
{
    if (enemigo->estatico || enemigo->muerto)  // No tiene sentido mover un enemigo estático o que ya murió
    {
        return;
    }
    
    if ((enemigo->direccion == -1 && hay_colision_izquierda(enemigo, mapa)) || (enemigo->direccion == 1 && hay_colision_derecha(enemigo, mapa)))
    {
        enemigo->direccion *= -1;
    }

    enemigo->posicion.x += enemigo->velocidad.x * enemigo->direccion;

    if (enemigo->fps_en_caminata % 8 == 0)
    {
        actualizar_frame(enemigo, '\0');  // El modo es '\0' ya que da igual para los enemigos
    }

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

Procedure aplicar_danho(Personaje* personaje, Natural cantidad_danho)
{
    if (personaje->subvida_actual <= cantidad_danho)
    {
        personaje->subvida_actual = 0;
        personaje->muerto = true;
    }

    else
    {
        personaje->subvida_actual -= DANHO_RAYO;
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

Procedure detectar_si_personaje_en_zona_de_rayo(Personaje* personaje, Rayo rayo[MAX_RAYOS])
{
    Natural i;

    if (personaje->muerto)
    {
        return;
    }

    if (personaje->danhado)  // Si el personaje ya habia sido dañado, solo se aumenta el tiempo que dura el daño (parpadeo)
    {
        if (personaje->tiempo_danho <= MAX_TIEMPO_INMUNE)
        {
            personaje->tiempo_danho += 1./FPS;
        }

        else
        {
            personaje->danhado = false;
            personaje->tiempo_danho = 0;
        }

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
        if (balas[i].direccion != 0)
        {
            return true;
        }
    }

    return false;
}


Procedure mover_balas_activas(Personaje* atacante, Personaje* victima, Mapa mapa)
{
    Natural i;

    for (i=0; i<MAX_BALAS; i++)
    {
        if (atacante->balas[i].direccion != 0)
        {
            atacante->balas[i].posicion.x += atacante->balas[i].velocidad.x;

            if (atacante->balas[i].posicion.x - RADIO_AXIAL_X_BALA > ANCHO_VENTANA || atacante->balas[i].posicion.x + RADIO_AXIAL_X_BALA < 0)
            {
                atacante->balas[i].direccion = 0;  // Se desactiva la bala
                atacante->balas[i].velocidad = VECTOR_NULO;  // Se deja de mover
                continue;
            }

            // Verificar colisión con Woofson   
            if (!victima->danhado)
            {
                if (atacante->balas[i].posicion.x + RADIO_AXIAL_X_BALA > victima->posicion.x &&
                    atacante->balas[i].posicion.x - RADIO_AXIAL_X_BALA < victima->posicion.x + victima->ancho &&
                    atacante->balas[i].posicion.y + RADIO_AXIAL_Y_BALA > victima->posicion.y &&
                    atacante->balas[i].posicion.y - RADIO_AXIAL_Y_BALA < victima->posicion.y + victima->alto)
                {
                    atacante->balas[i].direccion = 0;
                    atacante->balas[i].velocidad = VECTOR_NULO;

                    victima->danhado = true;
                    victima->tiempo_danho = 0;

                    aplicar_danho(victima, DANHO_BALA);

                    continue;
                }
            }

            al_draw_filled_ellipse(atacante->balas[i].posicion.x, atacante->balas[i].posicion.y, RADIO_AXIAL_X_BALA, RADIO_AXIAL_Y_BALA, VERDE);
        }
    }
}


bool puede_disparar_horizontalmente(Personaje enemigo, Personaje woofson, Mapa mapa)
{
    Natural fila, col;
    Natural col_enemigo, col_woofson, col_min, col_max;
    bool alineados_en_y;
    bool mirando_a_woofson;
    Natural frames_disparo[] = {0, 1, 2, 3, 7, 8, 9};
    Natural nro_frames_disparo = sizeof(frames_disparo) / sizeof(frames_disparo[0]);

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

    if (enemigo.bandera_dibujo == ALLEGRO_FLIP_HORIZONTAL && woofson.posicion.x + woofson.ancho <= enemigo.posicion.x)
    {
        mirando_a_woofson = true;
    }

    else if (enemigo.bandera_dibujo == 0 && woofson.posicion.x >= enemigo.posicion.x + enemigo.ancho)
    {
        mirando_a_woofson = true;
    }

    else
    {
        mirando_a_woofson = false;
    }

    return mirando_a_woofson;  // Puede disparar
}


Procedure efectuar_disparo_de_enemigo(Personaje* enemigo, Personaje* woofson, Mapa mapa)
{
    Natural i;
    float dx;

    if (!enemigo->inicializado) 
    {
        return;
    }

    if (hay_balas_activas(enemigo->balas))
    {
        mover_balas_activas(enemigo, woofson, mapa);
    }
    
    if (enemigo->frames_para_prox_disparo > 0)
    {
        enemigo->frames_para_prox_disparo--;
    }

    if (woofson->danhado)
    {
        if (woofson->tiempo_danho <= MAX_TIEMPO_INMUNE)
        {
            woofson->tiempo_danho += 1./FPS;
        }

        else
        {
            woofson->danhado = false;
            woofson->tiempo_danho = 0;
        }
    }

    if (puede_disparar_horizontalmente(*enemigo, *woofson, mapa))
    {
        for (i=0; i<MAX_BALAS; i++)  // Buscamos una bala disponible en el arreglo de balas del enemigo
        {
            if (enemigo->balas[i].velocidad.x == 0 && enemigo->balas[i].velocidad.y == 0)
            {
                // Inicializar posición y dirección de la bala
                enemigo->balas[i].posicion = (Vector) {enemigo->bandera_dibujo == 0 ? enemigo->posicion.x + enemigo->ancho : enemigo->posicion.x, 
                                                       enemigo->posicion.y + 0.45f * enemigo->alto};

                dx = woofson->posicion.x - enemigo->posicion.x;

                enemigo->balas[i].direccion = (dx >= 0) ? 1 : -1;
                enemigo->balas[i].velocidad.x = enemigo->balas[i].direccion * VELOCIDAD_BALA;
                enemigo->balas[i].velocidad.y = 0;
                enemigo->frames_para_prox_disparo = MAX_FRAMES_ESPERA;                
                // Sonido u otra animación si lo deseas
                // reproducir_sonido_disparo();
                break;
            }
        }
    }
}