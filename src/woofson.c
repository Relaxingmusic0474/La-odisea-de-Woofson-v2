#include "allegro.h"
#include "stdlib.h"
#include "time.h"
#include "characters.h"
#include "types.h"
#include "load.h"
#include "debug.h"

int main()
{
    Recursos recursos = {0};  /* Se inicializan los recursos del juego */
    ALLEGRO_EVENT evento; /* Ṕara agregar los eventos que vayan ocurriendo a la cola de eventos */
    Etapa etapa_juego = MENU_PRINCIPAL;  /* Para capturar el estado actual del juego */
    bool teclas[ALLEGRO_KEY_MAX] = {false}; /* Para manejar las teclas que se presionan */
    Natural ultima_tecla_lateral = ALLEGRO_KEY_RIGHT;  /* Para que el personaje parta mirando a la derecha */
    Natural iteracion = 0;  /* Para controlar las iteraciones del juego */
    
    if (!inicializar_todo(&recursos))  /* Se inicializan todos los recursos de Allegro */
    {
        return 1;
    }
    
    LOOP
    {
        al_wait_for_event(recursos.cola_eventos, &evento);  /* Se espera a que ocurra algun evento */
                
        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            break;  /* Se sale del loop si la ventana se cierra (el juego termina) */
        }

        if (etapa_juego == MENU_PRINCIPAL)
        {
            if (evento.type == CLICK)
            {
                recursos.menu_actual.opcion_en_hover = obtener_opcion_en_hover(recursos.menu_actual);

                if (recursos.menu_actual.opcion_en_hover < recursos.menu_actual.nro_opciones)
                {
                    redirigir_menu(&recursos, &recursos.menu_actual, recursos.fuentes[2], recursos.menu_actual.opcion_en_hover, &etapa_juego);
                }
            }

            else if (evento.type == ALLEGRO_EVENT_TIMER)
            {
                al_clear_to_color(NEGRO);
                mostrar_menu(recursos.menu_actual);
            }
        }

        else if (etapa_juego == MENU_NIVELES)
        {
            if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            {
                recursos.menu_actual.opcion_en_hover = obtener_opcion_en_hover(recursos.menu_actual);
        
                if (recursos.menu_actual.opcion_en_hover < recursos.menu_actual.nro_opciones)
                {
                    redirigir_menu(&recursos, &recursos.menu_actual, recursos.fuentes[3], recursos.menu_actual.opcion_en_hover, &etapa_juego);
                }
            }

            else if (evento.type == ALLEGRO_EVENT_TIMER)
            {
                al_clear_to_color(NEGRO);
                mostrar_menu(recursos.menu_actual);
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
                
                    determinar_color_pantalla(iteracion/*, &rojo, &verde, &azul*/);  // Determina el color de la pantalla según la iteración
                    mover_personaje(&recursos.pje_principal, teclas, recursos.mapas[NIVEL1]);  /* Mueve al personaje según las teclas presionadas y considera lógica de salto */
                    dibujar_personaje(recursos.pje_principal, teclas, ultima_tecla_lateral);  // Dibuja el personaje en su posición actual
                    dibujar_mapa(recursos.mapas[NIVEL1]);  // Dibujamos el mapa del primer nivel

                    if (iteracion % 10 == 0)
                    {
                        // mostrar_datos_personaje(recursos.pje_principal);
                        // mostrar_bloque_actual_personaje(recursos.pje_principal, recursos.mapas[NIVEL1]);
                        
                        recursos.pje_principal.subvida_actual--;

                        if (recursos.pje_principal.subvida_actual == 0)
                        {
                            recursos.pje_principal.nro_vidas--;
                            recursos.pje_principal.subvida_actual = 100;
                        }
                    }

                    mostrar_pantalla_datos(recursos.pje_principal, recursos.vida, recursos.fuentes[4], recursos.fuentes[1], etapa_juego);
				    
                    al_flip_display();

                    break;
            }            
        }

        iteracion = (iteracion + 1) % 60;
    }

    finalizar_allegro(&recursos);

    return 0;
}