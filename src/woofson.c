#include "allegro.h"
#include "types.h"

int main()
{
    Recursos recursos = {NULL};  /* Se inicializan los recursos del juego */
    ALLEGRO_EVENT evento; /* Ṕara agregar los eventos que vayan ocurriendo a la cola de eventos */
    Menu menu = {NULL}; /* Para manejar los menús */
    Posicion posicion_mouse;  /* Para capturar la posicion del raton, lo cual servira para los menús */
    Etapa etapa_juego = MENU_PRINCIPAL;  /* Para capturar el estado actual del juego */
    
    if (!inicializacion_allegro())
    {
        return 1;
    }

    if (!creacion_recursos_allegro(&recursos))  /* Se inicializan y crean los recursos esenciales para el juego (incluido el menu) */
    {
        finalizacion_allegro(&recursos);
        return 2;
    }

    al_set_window_title(recursos.ventana, "La odisea de Woofson");  /* Se personaliza el nombre de la ventana creada para
                                                                       que tenga como nombre el titulo del juego */

    al_start_timer(recursos.temporizador);  /* Se inicia el temporizador */

    LOOP
    {
        al_wait_for_event(recursos.cola_eventos, &evento);  /* Se espera a que ocurra algun evento */
                
        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            break;  /* Se sale del loop si la ventana se cierra (el juego termina) */
        }

        if (etapa_juego == MENU_PRINCIPAL)
        {
            if (!menu.inicializado)
            {
                if (!inicializar_menu_principal(&menu))
                {
                    finalizacion_allegro(&recursos);
                    return 3;
                }
            }

            else
            {
                if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                {
                    menu.opcion_en_hover = obtener_opcion_en_hover(menu);
            
                    if (menu.opcion_en_hover >= 0 && menu.opcion_en_hover < menu.nro_opciones)
                    {
                        redirigir_menu(&menu, menu.opcion_en_hover, &etapa_juego);
                    }
                }

                else if (evento.type == ALLEGRO_EVENT_TIMER)
                {
                    al_clear_to_color(NEGRO);
                    mostrar_menu(menu);
                }
            }
        }

        else if (etapa_juego == MENU_NIVELES)
        {
            if (!menu.inicializado)
            {
                if (!inicializar_menu_niveles(&menu))
                {
                    finalizacion_allegro(&recursos);
                    return 4;
                }
            }

            else
            {
                if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                {
                    menu.opcion_en_hover = obtener_opcion_en_hover(menu);
            
                    if (menu.opcion_en_hover >= 0 && menu.opcion_en_hover < menu.nro_opciones)
                    {
                        redirigir_menu(&menu, menu.opcion_en_hover, &etapa_juego);
                    }
                }

                else if (evento.type == ALLEGRO_EVENT_TIMER)
                {
                    al_clear_to_color(NEGRO);
                    mostrar_menu(menu);
                }
            }
        }
    }

    finalizacion_allegro(&recursos);

    return 0;
}