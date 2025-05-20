#include "allegro.h"

bool inicializacion_allegro()
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


bool creacion_recursos_allegro(Recursos* R)
{
    R->ventana = al_create_display(ANCHO_VENTANA, ALTO_VENTANA);

    if (!R->ventana)
    {
        printf("Error al crear la ventana.\n");
        return false;
    }

    al_set_window_title(R->ventana, NOMBRE_JUEGO);
    //al_set_window_position(R->ventana, 0, 0);

    R->cola_eventos = al_create_event_queue();

    if (!R->cola_eventos)
    {
        printf("Error al crear la cola de eventos.\n");
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

    al_register_event_source(R->cola_eventos, R->eventos.ventana);

    R->eventos.temporizador = al_get_timer_event_source(R->temporizador);

    if (!R->eventos.temporizador)
    {
        printf("No se pudieron obtener los eventos del temporizador.\n");
        return false;
    }

    al_register_event_source(R->cola_eventos, R->eventos.temporizador);

    R->eventos.teclado = al_get_keyboard_event_source();

    if (!R->eventos.teclado)
    {
        printf("No se pudieron obtener los eventos del teclado.\n");
        return false;
    }

    al_register_event_source(R->cola_eventos, R->eventos.teclado);

    R->eventos.raton = al_get_mouse_event_source();

    if (!R->eventos.raton)
    {
        printf("No se pudieron obtener los eventos del mouse.\n");
        return false;
    }

    al_register_event_source(R->cola_eventos, al_get_mouse_event_source());

    return true;
}


Procedure finalizacion_allegro(Recursos* R)
{
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