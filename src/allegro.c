#include "allegro.h"

/**
 * Función que inicializa Allegro5 y sus módulos necesarios.
 * @return true si se inicializó correctamente, false en caso contrario.
 */
bool inicializar_allegro()
{
    if (!al_init())
    {
        printf("Error al inicializar allegro.\n");
        return false;
    }

    if (!al_init_primitives_addon())
    {
        printf("Error al inicializar el modulo de primitivas.\n");
        return false;
    }
    
    if (!al_init_font_addon())
    {
        printf("Error al inicializar el modulo de fuentes.\n");
        return false;
    }

    if (!al_init_ttf_addon())
    {
        printf("Error al inicializar el modulo TTF.\n");
        return false;
    }

    if (!al_init_image_addon())
    {
        printf("Error al inicializar el modulo de imagenes.\n");
        return false;
    }

    if (!al_install_audio())
    {
        printf("Error al instalar el sistema de audio.\n");
        return false;
    }

    if (!al_init_acodec_addon())
    {
        printf("Error al inicializar el addon de codecs de audio.\n");
        return false;
    }

    if (!al_install_keyboard())
    {
        printf("Error al instalar el sistema de teclado.\n");
        return false;
    }

    if (!al_install_mouse())
    {
        printf("Error al instalar el sistema del raton.\n");
        return false;
    }

    if (!al_reserve_samples(NRO_MUESTRAS_AUDIO_RESERVADAS))  /* Reservamos 10 muestras de audio para evitar problemas de memoria */
    {
        printf("Error al reservar las muestras de audio.\n");
        return false;
    }  

    return true;
}


/**
 * Función que crea los recursos necesarios para el juego.
 * Crea la ventana, la cola de eventos y el temporizador, y también lee los mapas de los niveles.
 * @param R Puntero a la estructura Recursos donde se almacenarán los recursos creados.
 * @return true si se crearon los recursos correctamente, false en caso contrario. 
 */ 
bool crear_recursos_allegro(Recursos* R)
{
    Natural i;

    /* Se crea la ventana */
    R->ventana = al_create_display(ANCHO_VENTANA, ALTO_VENTANA);

    if (!R->ventana)
    {
        printf("Error al crear la ventana.\n");
        return false;
    }

    /* Se ajusta el título de la ventana para que tenga e nombre del juego */
    al_set_window_title(R->ventana, NOMBRE_JUEGO);  
    // También se podría usar al_set_window_position() para posicionar la ventana en una ubicación específica

    R->cola_eventos = al_create_event_queue();

    if (!R->cola_eventos)
    {
        printf("Error al crear la cola de eventos.\n");
        al_destroy_display(R->ventana);
        R->ventana = NULL;
        return false;
    }

    R->temporizador = al_create_timer(1./FPS);

    if (!R->temporizador)
    {
        printf("Error al crear el temporizador.\n");
        return false;
    }

    al_start_timer(R->temporizador);

    R->eventos.ventana = al_get_display_event_source(R->ventana);

    if (!R->eventos.ventana)
    {
        printf("No se pudieron obtener los eventos de la ventana.\n");
        return false;
    }

    /* Se registran los eventos de la ventana en la cola de eventos */
    al_register_event_source(R->cola_eventos, R->eventos.ventana);

    R->eventos.temporizador = al_get_timer_event_source(R->temporizador);

    if (!R->eventos.temporizador)
    {
        printf("No se pudieron obtener los eventos del temporizador.\n");
        return false;
    }

    /* Se registran los eventos del temporizador en la cola de eventos */
    al_register_event_source(R->cola_eventos, R->eventos.temporizador);

    R->eventos.teclado = al_get_keyboard_event_source();

    if (!R->eventos.teclado)
    {
        printf("No se pudieron obtener los eventos del teclado.\n");
        return false;
    }

    /* Se registran los eventos del teclado en la cola de eventos */
    al_register_event_source(R->cola_eventos, R->eventos.teclado);

    R->eventos.raton = al_get_mouse_event_source();

    if (!R->eventos.raton)
    {
        printf("No se pudieron obtener los eventos del mouse.\n");
        return false;
    }

    /* Se registran los eventos del mouse en la cola de eventos */
    al_register_event_source(R->cola_eventos, al_get_mouse_event_source());

    /* Se leen los mapas de todos los niveles */
    for (i=0; i<NRO_NIVELES; i++)
    {
        R->mapas[i] = leer_mapa(i+1);  // Se leen los mapas de los niveles, comenzando desde el nivel 1 (índice 0)

        /* Si el mapa es NULL, se imprime un mensaje de error y se liberan los mapas ya leidos antes de retornar false */
        if (R->mapas[i].mapa == NULL)
        {
            printf("Error al leer el mapa del nivel %d.\n", i);
            liberar_mapas(R->mapas);  /* Liberar los mapas ya leídos antes de retornar false */
            return false;
        }
    }

    return true;
}


/**
 * Función que inicializa todos los módulos y recursos de Allegro5 necesarios para el juego, además de inicializar al personaje principal.
 * @param R Puntero a la estructura Recursos donde se almacenarán los recursos creados.
 * @return true si se inicializaron todos los recursos correctamente, false en caso contrario.
 */
bool inicializar_todo(Recursos* R, Personaje* P)
{
    /* Se inicializa la semilla para los números aleatorios */
    srand(time(NULL));

    /* Se inicializan todos los módulos de allegro */
    if (!inicializar_allegro())
    {
        finalizar_allegro(R);  // Por si el error ocurre después de haber inicializado algunos módulos
        return false;
    }

    /* Se crean los recursos necesarios para el juego (ventana, cola de eventos, temporizador, mapa, etc) */
    if (!crear_recursos_allegro(R))
    {
        finalizar_allegro(R);  // En caso de error, se liberan los recursos creados
        return false;
    }

    /* Se inicializa al personaje con un identificador */
    if (!inicializar_personaje(P, 'D'))
    {
        finalizar_allegro(R);
        return false;
    }

    return true;
}


/**
 * Función que libera todos los recursos utilizados por Allegro5.
 * Destruye la ventana, la cola de eventos y el temporizador.
 * @param R Puntero a la estructura Recursos donde se almacenan los recursos a liberar.
 */
Procedure finalizar_allegro(Recursos* R)
{
    liberar_mapas(R->mapas);

    if (R->cola_eventos != NULL)
    {
        al_destroy_event_queue(R->cola_eventos);
        R->cola_eventos = NULL;
    }
    
    if (R->temporizador != NULL)
    {
        al_destroy_timer(R->temporizador);
        R->temporizador = NULL;
    }
    
    if (R->ventana != NULL)
    {
        al_destroy_display(R->ventana);
        R->ventana = NULL;
    }

    al_uninstall_system();

    return;
}

void mostrar_info(Personaje dragon, Natural iteracion)
{
    if (iteracion % 20 == 0)
    {
        printf("Posicion: (%.2f, %.2f) | Velocidad: (%.2f, %.2f) | Impulso: %.2f | Tiempo en salto: %.2f\n",
        dragon.posicion.x, dragon.posicion.y, dragon.velocidad.x, dragon.velocidad.y,
        dragon.salto.impulso, dragon.salto.tiempo_en_salto);
    }
}