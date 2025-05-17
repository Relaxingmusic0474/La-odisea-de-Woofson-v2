#include "allegro.h"
#include "types.h"
#include "load.h"

int main()
{
    Recursos recursos = {NULL};  /* Se inicializan los recursos del juego */
    ALLEGRO_EVENT evento; /* Ṕara agregar los eventos que vayan ocurriendo a la cola de eventos */
    Menu menu = {NULL}; /* Para manejar los menús */
    Etapa etapa_juego = MENU_PRINCIPAL;  /* Para capturar el estado actual del juego */
    Musica* musica = {NULL}; /* Para manejar la música del juego */
    Personaje personaje = {NULL}; /* Para manejar el personaje del juego */
    
    if (!inicializacion_allegro()) /* Se inicializan todos los módulos de la librería Allegro5 */
    {
        return 1;
    }

    if (!creacion_recursos_allegro(&recursos))  /* Se inicializan y crean los recursos esenciales para el juego (incluido el menu) */
    {
        finalizacion_allegro(&recursos);
        return 2;
    }

    musica = cargar_musica(MUSICA_MENU);  /* Se carga la música del menú */

    if (!musica)
    {
        finalizacion_allegro(&recursos);
        return 3;
    }

    al_play_sample(musica->musica, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &musica->ID);  /* Se inicia la música del menú */

    LOOP
    {
        al_wait_for_event(recursos.cola_eventos, &evento);  /* Se espera a que ocurra algun evento */
                
        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            break;  /* Se sale del loop si la ventana se cierra (el juego termina) */
        }

        if (etapa_juego == MENU_PRINCIPAL)
        {
            if (!menu.inicializado) /* Se hace esta comprobación para que no se inicialice varias veces */
            {
                if (!inicializar_menu_principal(&menu))
                {
                    finalizacion_allegro(&recursos);
                    return 4;
                }
            }

            else
            {
                if (evento.type == CLICK)
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
                    return 5;
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

        else if (etapa_juego == NIVEL1)
        {
            al_stop_sample(&musica->ID);
        }
    }

    al_destroy_sample(musica->musica);
    free(musica);
    finalizacion_allegro(&recursos);

    return 0;
}