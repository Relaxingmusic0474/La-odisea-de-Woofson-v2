#include "allegro.h"
#include "stdlib.h"
#include "time.h"
#include "types.h"

int main()
{
    Posicion posicion_mouse;  /* Para capturar la posicion del raton, lo cual servira para el menu */
    Natural iteracion = 0;
    ALLEGRO_EVENT evento;
    
    if (!inicializacion_allegro())
    {
        return 1;
    }
    
    Recursos recursos = {NULL};  /* Como la variable de los recursos depende de la inicializacion de allegro, 
                                    se declaro despues de la inicializacion */

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

        else if (evento.type == ALLEGRO_EVENT_TIMER)
        {
            al_clear_to_color(NEGRO);
            mostrar_menu(recursos.menu);
            iteracion++;
        }
    }

    finalizacion_allegro(&recursos);

    return 0;
}