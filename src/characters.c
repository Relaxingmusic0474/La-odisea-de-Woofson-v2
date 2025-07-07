#include "characters.h"

/**
 * Función que inicializa un personaje según su tipo.
 * @param personaje Puntero al personaje a inicializar.
 * @param tipo Tipo de personaje a inicializar ('W' para Woofson, 'D' para Dragón, 'H' para Humanoide, 'J' para Jefe).
 * @return true si se inicializó correctamente, false en caso contrario.
 */
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

    personaje->nro_vidas = VIDAS_INICIALES;
    personaje->escala_dibujo = 1.0;
    personaje->ancho = al_get_bitmap_width(personaje->imagen) * personaje->escala_dibujo;  // Ancho del personaje
    personaje->alto = al_get_bitmap_height(personaje->imagen) * personaje->escala_dibujo;  // Alto del personaje
    personaje->posicion.x = ANCHO_VENTANA * 0.1;
    personaje->posicion.y = ALTURA_PISO - personaje->alto;  // Se coloca en el piso
    personaje->bandera_dibujo = 0;  // 0: normal, ALLEGRO_FLIP_HORIZONTAL: espejo
    personaje->en_plataforma = false;  // Inicialmente no está en una plataforma (está en el suelo)
    inicializar_salto(personaje);  // Inicializa la estructura del salto para el personaje

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
Procedure determinar_como_dibujar_personaje(Personaje* personaje, bool teclas[ALLEGRO_KEY_MAX], Natural ultima_tecla_lateral)
{
    /* Se determina cómo se debe dibujar el personaje según las teclas presionadas */
    if (teclas[ALLEGRO_KEY_LEFT] || (!teclas[ALLEGRO_KEY_RIGHT] && ultima_tecla_lateral == ALLEGRO_KEY_LEFT))
    {
        personaje->bandera_dibujo = ALLEGRO_FLIP_HORIZONTAL;  // Dibuja el personaje mirando a la izquierda
    }

    else
    {
        personaje->bandera_dibujo = 0;  // Dibuja el personaje mirando a la derecha
    }

    return;
}


/**
 * Función que dibuja el personaje en su posición actual.
 * @param personaje El personaje a dibujar.
 * @param teclas Un arreglo de booleanos que indica qué teclas están presionadas.
 * @param ultima_tecla_lateral La última tecla lateral presionada (para determinar la dirección de dibujo).
 */
Procedure dibujar_personaje(Personaje personaje, bool teclas[ALLEGRO_KEY_MAX], Natural ultima_tecla_lateral)
{
    /* Se determina cómo se debe dibujar el personaje según las teclas presionadas */
    determinar_como_dibujar_personaje(&personaje, teclas, ultima_tecla_lateral);

    /* Se dibuja el personaje en su posición actual */
    al_draw_bitmap(personaje.imagen, personaje.posicion.x, personaje.posicion.y, personaje.bandera_dibujo);

    return;
}


/**
 * Función para plasmar la lógica del movimiento del personaje según las teclas presionadas y el mapa.
 * También maneja la lógica del salto y las colisiones con el mapa.
 * @param personaje Puntero al personaje que se va a mover.
 * @param teclas Un arreglo de booleanos que indica qué teclas están presionadas.
 * @param mapa El mapa del juego, que contiene los bloques y obstáculos.
 */
Procedure mover_personaje(Personaje* personaje, bool teclas[ALLEGRO_KEY_MAX], Mapa mapa)
{
    if (teclas[ALLEGRO_KEY_UP] && !personaje->salto.en_salto && !hay_colision_superior(*personaje, mapa))
    {
        personaje->salto.en_salto = true;  /* Activa el salto */
        // personaje->posicion.y--;  /* Para que no quede pegado al suelo al saltar parte 1 px sobre el suelo (no se notará) */
        personaje->salto.altura_inicial = personaje->posicion.y;  /* Guarda la altura inicial del salto */
        personaje->salto.tiempo_en_salto = 0;  /* Reinicia el tiempo de salto */
    }
    
    if (teclas[ALLEGRO_KEY_DOWN] && !hay_colision_inferior(personaje, mapa) && !hay_bloque_debajo(personaje, mapa))
    {
        personaje->posicion.y += 4;  /* Por si el personaje está en el aire y quiere bajar un poco más rápido */
    }   

    if (teclas[ALLEGRO_KEY_LEFT] && !hay_colision_izquierda(*personaje, mapa))
    {
        personaje->posicion.x -= personaje->velocidad.x;

        if (!personaje->salto.en_salto && personaje->posicion.y + personaje->alto < ALTURA_PISO && !hay_bloque_debajo(personaje, mapa))
        {
            activar_caida_libre(personaje);  /* Activa la caída libre si el personaje no está en el suelo */
        }
    }

    if (teclas[ALLEGRO_KEY_RIGHT] && !hay_colision_derecha(*personaje, mapa))
    {
        personaje->posicion.x += personaje->velocidad.x;

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

    return;
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

    if (hay_colision_superior(*personaje, mapa))
    {
        personaje->salto.es_interrumpido = true;  /* Marca que el salto es interrumpido por la colisión con el techo */
        personaje->posicion.y = 0;
        personaje->salto.tiempo_en_salto = 0;  /* Reinicia el tiempo de salto para evitar problemas de acumulación */
        personaje->salto.altura_choque = personaje->posicion.y;  /* Guarda la altura del choque con el techo */
        personaje->salto.altura_inicial = personaje->salto.altura_choque;
        personaje->salto.impulso = personaje->velocidad.y;  /* Reinicia el impulso del salto */
        personaje->velocidad.y = -personaje->velocidad.y;  /* Invierte la velocidad en el eje y */
    }

    /* Si el personaje colisiona ya sea con el suelo o con algún bloque sólido u objeto, se detiene el salto */
    if (hay_colision_inferior(personaje, mapa))
    {
        efectuar_colision(personaje, mapa);  /* Efectúa la colisión con el suelo o bloque */
    }

    return;
}


/**
 * Función que simula la colisión del personaje con el techo, invirtiendo su velocidad en el eje y.
 * @param personaje El personaje que colisiona con el techo.
 * @param altura_techo La altura del techo con el que colisiona el personaje.
 */
Procedure colisionar_con_techo(Personaje* personaje, Entero altura_techo)
{
    personaje->velocidad.y = -personaje->velocidad.y;  /* Aplicando la 3ra ley de Newton, se invierte la velocidad en y */
    personaje->posicion.y = altura_techo;  /* Ajusta la posición del personaje al techo */
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