#include "allegro.h"
#include "stdlib.h"
#include "time.h"
#include "types.h"

int main()
{
    Posicion posicion_mouse;  /* Para capturar la posicion del raton, lo cual servira para el menu */
    Natural iteracion = 0;
    ALLEGRO_EVENT evento;
    Recursos recursos = {NULL};  /* Se inicializan los recursos del juego */
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
            if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                recursos.menu.opcion_en_hover = obtener_opcion_en_hover(recursos.menu);

                if (recursos.menu.opcion_en_hover >= 0 && recursos.menu.opcion_en_hover < NRO_OPCIONES_MENU)
                {
                    redirigir_menu(&recursos.menu, recursos.menu.opcion_en_hover, &etapa_juego);
                }
            }

            else if (evento.type == ALLEGRO_EVENT_TIMER)
            {
                al_clear_to_color(NEGRO);
                mostrar_menu(recursos.menu);
                iteracion++;
            }
        }

        else if (etapa_juego == MENU_NIVELES)
        {

        }

        else if (etapa_juego == INSTRUCCIONES)
        {

        }
    }

    finalizacion_allegro(&recursos);

    return 0;
}