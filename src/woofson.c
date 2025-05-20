#include "allegro.h"
#include "time.h"
#include "characters.h"
#include "types.h"
#include "load.h"

int main()
{
    Recursos recursos = {NULL};  /* Se inicializan los recursos del juego */
    ALLEGRO_EVENT evento; /* Ṕara agregar los eventos que vayan ocurriendo a la cola de eventos */
    Menu menu = {NULL}; /* Para manejar los menús */
    Etapa etapa_juego = MENU_PRINCIPAL;  /* Para capturar el estado actual del juego */
    Musica* musica = {NULL}; /* Para manejar la música del juego */
    Personaje dragon = {NULL}; /* Para manejar el personaje del juego */
    bool teclas[ALLEGRO_KEY_MAX] = {false}; /* Para manejar las teclas que se presionan */
    Natural ultima_tecla_lateral = ALLEGRO_KEY_RIGHT;
    Natural iteracion = 0;
    Natural rojo, verde, azul;
    double t;

    srand(time(NULL));  /* Se inicializa la semilla para los números aleatorios */
    
    if (!inicializacion_allegro()) /* Se inicializan todos los módulos de la librería Allegro5 */
    {
        return 1;
    }

    if (!creacion_recursos_allegro(&recursos))  /* Se inicializan y crean los recursos esenciales para el juego (incluido el menu) */
    {
        finalizacion_allegro(&recursos);
        return 2;
    }

    if (!inicializar_personaje(&dragon, 'D'))  /* Se inicializa el personaje */
    {
        finalizacion_allegro(&recursos);
        return 3;
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
            switch (evento.type)
            {
                case TECLA_PRESIONADA:

                    teclas[evento.keyboard.keycode] = true;
                    
                    if (es_tecla_lateral(evento.keyboard.keycode))
                    {
                        ultima_tecla_lateral = (Natural) evento.keyboard.keycode;
                    }
                    
                    break;


                case TECLA_LIBERADA:

                    teclas[evento.keyboard.keycode] = false;

                    if (es_tecla_lateral(evento.keyboard.keycode))
                    {
                        ultima_tecla_lateral = (Natural) evento.keyboard.keycode;
                    }

                    break;


                case ALLEGRO_EVENT_TIMER:

                    if (dragon.en_salto)
                    {
                        dragon.tiempo_salto += 1.0 / FPS;
                        continuar_salto(&dragon, dragon.tiempo_salto);
                    }

                    /*
                    else
                    {*/
                        mover_personaje(&dragon, teclas);
                    // }
                    
                    if (iteracion == 0)
                    {
                        // Aquí puedes agregar la lógica que deseas ejecutar cada 30 frames
                        rojo = rand() % 256;
                        verde = rand() % 256;
                        azul = rand() % 256;
                    }

                    al_clear_to_color(al_map_rgb(rojo, verde, azul));

                    if (teclas[ALLEGRO_KEY_LEFT] || (!teclas[ALLEGRO_KEY_RIGHT] && ultima_tecla_lateral == ALLEGRO_KEY_LEFT))
				    {
					    dragon.bandera_dibujo = ALLEGRO_FLIP_HORIZONTAL;
				    }

				    else
				    {
					    dragon.bandera_dibujo = 0;
				    }

                    al_draw_bitmap(dragon.imagen, dragon.posicion.x, dragon.posicion.y, dragon.bandera_dibujo);
				    // al_draw_rotated_bitmap(dragon.imagen, al_get_bitmap_width(dragon.imagen) / 2, al_get_bitmap_height(dragon.imagen), dragon.posicion.x, dragon.posicion.y, ALLEGRO_PI/2, dragon.bandera_dibujo);
				    al_flip_display();

                    break;
            }

            // al_flip_display();
        }

        iteracion = (iteracion + 1) % 30;
    }

    al_destroy_sample(musica->musica);
    free(musica);
    musica = NULL;
    al_destroy_bitmap(dragon.imagen);
    dragon.imagen = NULL;
    finalizacion_allegro(&recursos);

    return 0;
}