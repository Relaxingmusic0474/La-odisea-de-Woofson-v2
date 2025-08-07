#include "allegro.h"
#include "load.h"
#include "debug.h"

int main()
{
    Recursos recursos = {0};                        /* Se inicializan los recursos del juego */
    ALLEGRO_EVENT evento;                           /* Ṕara agregar los eventos que vayan ocurriendo a la cola de eventos */
    Etapa etapa_juego = MENU_PRINCIPAL;             /* Para capturar el estado actual del juego */
    Tecla ultima_tecla_lateral = ALLEGRO_KEY_RIGHT; /* Para que el personaje parta mirando a la derecha */
    Natural iteracion = 0, nivel_actual = 0;        /* Para controlar las iteraciones del juego */
    bool cambio_estado_procesado = false;           /* Para el estado de la puerta y la palanca */
    float tiempo_en_nivel = 0;                      /* Para manejar el tiempo que se lleva en el nivel */
    Natural ranking_a_mostrar = -1;                 /* Este valor es equivalente a la macro USHRT_MAX (65535) -> No hay ranking inicial que se muestre */
    
    // Se inicializan todos los recursos de Allegro
    if (!inicializar_todo(&recursos))
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

        if (etapa_juego == MENU_PRINCIPAL || etapa_juego == MENU_NIVELES || etapa_juego == MENU_RANKING || etapa_juego == RANKING || etapa_juego == VICTORIA || etapa_juego == DERROTA)
        {
            manejar_menu(&recursos, &evento, &etapa_juego, &nivel_actual, iteracion, &cambio_estado_procesado, &ultima_tecla_lateral, &ranking_a_mostrar, &tiempo_en_nivel);
        }

        else
        {
            switch (evento.type)
            {
                case TECLA_PRESIONADA:

                    teclas[evento.keyboard.keycode] = recursos.pje_principal.muerto ? false : true;

                    if (!recursos.pje_principal.muerto && es_tecla_lateral(evento.keyboard.keycode))
                    {
                        ultima_tecla_lateral = evento.keyboard.keycode;
                    }

                    break;

                case TECLA_LIBERADA:
                
                    teclas[evento.keyboard.keycode] = false;

                    if (!recursos.pje_principal.muerto && es_tecla_lateral(evento.keyboard.keycode))
                    {
                        ultima_tecla_lateral = evento.keyboard.keycode;
                    }

                    break;

                case ALLEGRO_EVENT_TIMER:

                    mostrar_fondo_nivel(recursos.fondos, nivel_actual, iteracion);
                    tiempo_en_nivel += 1./FPS;

                    if (nivel_actual != 0)
                    {
                        mover_personaje(&recursos.pje_principal, recursos.mapas[nivel_actual - 1], nivel_actual - 1); // Mueve al personaje según las teclas presionadas y considera lógica de salto
                        recursos.pje_principal.balas_disponibles = nro_balas_disponibles(recursos.pje_principal.balas);
                        efectuar_disparo_de_woofson(&recursos.pje_principal, recursos.enemigos, recursos.mapas[nivel_actual - 1], nivel_actual);
                        dibujar_mapa(recursos.mapas[nivel_actual - 1], &recursos, &cambio_estado_procesado, iteracion); // Dibujamos el mapa del primer nivel
                        actualizar_estado_danho(&recursos.pje_principal);
                        actualizar_estado_danho_enemigos(recursos.enemigos);
                        dibujar_personaje(&recursos.pje_principal, ultima_tecla_lateral, iteracion);                    // Dibuja el personaje en su posición actual
                        actualizar_rayos(recursos.rayos[nivel_actual - 1], recursos.cantidad_rayos[nivel_actual - 1], recursos.pje_principal, recursos.mapas[nivel_actual - 1]);
                        detectar_si_personaje_en_zona_de_rayo(&recursos.pje_principal, recursos.rayos[nivel_actual - 1]);
                        efectuar_disparo_de_enemigos(recursos.enemigos, &recursos.pje_principal, recursos.mapas[nivel_actual - 1], nivel_actual, recursos.fuego);
                        determinar_victoria_woofson(&recursos.pje_principal, recursos.enemigos, recursos.puerta, &etapa_juego);

                        if (recursos.pje_principal.victoria)
                        {
                            etapa_juego = VICTORIA;
                        }

                        morir(&recursos.pje_principal, &ultima_tecla_lateral, &etapa_juego);

                        if (recursos.pje_principal.nro_vidas == 0 && recursos.pje_principal.subvida_actual == 0) // Perdió
                        {
                            etapa_juego = DERROTA;
                        }
                    
                        mostrar_pantalla_datos(recursos.pje_principal, recursos.municiones[0], recursos.vida, recursos.fuentes[COMFORTAA_LIGHT_GIGANTE],
                                               recursos.fuentes[TIMES_NEW_ROMAN_NORMAL], nivel_actual, tiempo_en_nivel);

                        if (teclas[ALLEGRO_KEY_ESCAPE])
                        {
                            detener_efectos_de_sonido(&recursos);
                            cambiar_menu(&recursos.menu_actual, recursos.menus[PRINCIPAL]);
                            cambiar_musica(&recursos, recursos.musicas[0]);
                            nivel_actual = 0;
                            resetear_estado_juego(&recursos, recursos.menu_actual, &etapa_juego, MENU_PRINCIPAL, &tiempo_en_nivel);
                        }
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