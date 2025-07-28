#include "allegro.h"
#include "characters.h"
#include "types.h"
#include "load.h"
#include "debug.h"

int main()
{
    Recursos recursos = {0};  /* Se inicializan los recursos del juego */
    ALLEGRO_EVENT evento; /* Ṕara agregar los eventos que vayan ocurriendo a la cola de eventos */
    Etapa etapa_juego = MENU_PRINCIPAL;  /* Para capturar el estado actual del juego */
    Tecla ultima_tecla_lateral = ALLEGRO_KEY_RIGHT;  /* Para que el personaje parta mirando a la derecha */
    Natural iteracion = 0, nivel_actual = 0;  /* Para controlar las iteraciones del juego */
    extern bool teclas[ALLEGRO_KEY_MAX]; /* Para manejar las teclas que se presionan */
    bool cambio_estado_procesado = false;

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

        if (etapa_juego == MENU_PRINCIPAL || etapa_juego == MENU_NIVELES || etapa_juego == DERROTA || etapa_juego == VICTORIA)
        {
            manejar_menu(&recursos, &evento, &etapa_juego, &nivel_actual, iteracion, &cambio_estado_procesado, &ultima_tecla_lateral);
        }
        
        else
        {
            switch (evento.type)
            {
                case TECLA_PRESIONADA:

                    teclas[evento.keyboard.keycode] = recursos.pje_principal.muerto ? false : true;
                    
                    if (!recursos.pje_principal.muerto && es_tecla_lateral(evento.keyboard.keycode))
                    {
                        ultima_tecla_lateral = (Natural) evento.keyboard.keycode;
                    }
                    
                    break;


                case TECLA_LIBERADA:

                    teclas[evento.keyboard.keycode] = false;

                    if (!recursos.pje_principal.muerto && es_tecla_lateral(evento.keyboard.keycode))
                    {
                        ultima_tecla_lateral = (Natural) evento.keyboard.keycode;
                    }

                    break;


                case ALLEGRO_EVENT_TIMER:

                    al_clear_to_color(NEGRO);
                    mostrar_fondo_nivel(recursos.fondos, nivel_actual, iteracion);
                    
                    if (nivel_actual != 0)
                    {
                        mover_personaje(&recursos.pje_principal, recursos.mapas[nivel_actual-1], nivel_actual-1);  // Mueve al personaje según las teclas presionadas y considera lógica de salto
                        efectuar_disparo_de_woofson(&recursos.pje_principal, recursos.enemigos, recursos.mapas[nivel_actual-1]);
                        actualizar_estado_danho(&recursos.pje_principal);
                        actualizar_estado_danho_enemigos(recursos.enemigos);
                        dibujar_mapa(recursos.mapas[nivel_actual-1], &recursos, &cambio_estado_procesado, iteracion);  // Dibujamos el mapa del primer nivel
                        dibujar_personaje(&recursos.pje_principal, ultima_tecla_lateral, iteracion);  // Dibuja el personaje en su posición actual
                        actualizar_rayos(recursos.rayos[nivel_actual-1], recursos.cantidad_rayos[nivel_actual-1], recursos.pje_principal, recursos.mapas[nivel_actual-1]);
                        detectar_si_personaje_en_zona_de_rayo(&recursos.pje_principal, recursos.rayos[nivel_actual-1]);
                        efectuar_disparo_de_enemigos(recursos.enemigos, &recursos.pje_principal, recursos.mapas[nivel_actual-1]);
                        determinar_victoria_woofson(&recursos.pje_principal, recursos.enemigos, recursos.puerta, &etapa_juego);

                        if (recursos.pje_principal.victoria)
                        {
                            etapa_juego = VICTORIA;
                        }

                        morir(&recursos.pje_principal, &ultima_tecla_lateral, &etapa_juego);

                        if (recursos.pje_principal.nro_vidas == 0 && recursos.pje_principal.subvida_actual == 0)  // Perdió
                        {
                            etapa_juego = DERROTA;
                        }

                        mostrar_pantalla_datos(recursos.pje_principal, recursos.vida, recursos.fuentes[COMFORTAA_LIGHT_GIGANTE], 
                                               recursos.fuentes[TIMES_NEW_ROMAN_NORMAL], nivel_actual);
                    }
                    
                    al_flip_display();

                    break;
            }            
        }

        iteracion = (iteracion + 1) % 60;
    }

    finalizar_allegro(&recursos);
    return 0;
}