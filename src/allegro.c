#include "allegro.h"

bool teclas[ALLEGRO_KEY_MAX] = {false};  // Arreglo para manejar las teclas presionadas

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
    Natural i, j;
    char ruta[30] = {'\0'};
    bool exito = false;

    /* Se crea la ventana */
    R->ventana = al_create_display(ANCHO_VENTANA, ALTO_VENTANA);

    if (!R->ventana)
    {
        printf("Error al crear la ventana.\n");
        return false;
    }

    al_set_window_title(R->ventana, NOMBRE_JUEGO);  // Se ajusta el título de la ventana para que tenga e nombre del juego
    al_set_window_position(R->ventana, 0, 0);  // Se posiciona la ventana en la esquina superior izquierda de la pantalla

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
        if (!R->mapas[i].mapa)
        {
            printf("Error al leer el mapa del nivel %d.\n", i);
            return false;
        }
    }

    if (!inicializar_personaje(&R->pje_principal, 'W'))
    {
        return false;
    }

    R->vida = al_load_bitmap("assets/images/corazon.png");

    if (!R->vida)
    {
        printf("Error al cargar la imagen de las vidas del personaje\n");
        return false;
    }

    for (i=0; i<NRO_BLOQUES; i++)
    {
        sprintf(ruta, "assets/images/bloque-%hu.png", i+1);
        R->bloques[i] = al_load_bitmap(ruta);

        if (!R->bloques[i])
        {
            printf("Error al cargar la imagen del %hu' bloque del juego\n", i);
            return false;
        }

        memset(ruta, '\0', sizeof(ruta));
    }

    /* Se cargan las fuentes */
    for (i=0; i<3; i++)
    {
        for (j=0; j<2; j++)
        {
            R->fuentes[2*i+j] = cargar_fuente((Fuente) j, i==0 ? NORMAL : (i==1 ? GRANDE : GIGANTE));
            
            if (!R->fuentes[2*i+j])
            {
                printf("Error al cargar la fuente %s de tamaño %hu\n", 
                        j==0 ? "Comfortaa-Light" : "Times New Roman", 
                        i==0 ? NORMAL : (i==1 ? GRANDE : GIGANTE));
                return false;
            }
            
            printf("Se cargó la fuente %s de tamaño %hu\n", 
                        j==0 ? "Comfortaa-Light" : "Times New Roman", 
                        i==0 ? NORMAL : (i==1 ? GRANDE : GIGANTE));
        }
    }

    /* Se cargan los menus */
    for (i=0; i<NRO_MENUS; i++)
    {
        exito = i==0 ? inicializar_menu_principal(&R->menus[i], R->fuentes[3]) : inicializar_menu_niveles(&R->menus[i], R->fuentes[2]);

        if (!exito)
        {
            // printf("Error al cargar %s\n", i==0 ? "el menú principal" : "el menú de niveles");
            return false;
        }
    }

    R->menu_actual = R->menus[0];  // Menú principal

    // Se crea la voz
    R->voz = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);

    if (!R->voz)
    {
        printf("Error al crear la voz.\n");
        return false;
    }

    // Se crea el mezclador de sonidos
    R->mixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);

    if (!R->mixer)
    {
        printf("Error al crear el mixer.\n");
        return false;
    }

    // Conectar mixer a la voz
    if (!al_attach_mixer_to_voice(R->mixer, R->voz)) 
    {
        printf("Error al intentar conectar el mixer a la voz\n");
        al_destroy_mixer(R->mixer);
        al_destroy_voice(R->voz);
        R->mixer = NULL;
        R->voz = NULL;
        return false;
    }

    for (i=0; i<NRO_MUSICAS; i++)
    {
        R->musicas[i] = i==0 ? cargar_musica(MUSICA_MENU, R->mixer) : cargar_musica(MUSICA_NIVEL_1, R->mixer);
        
        if (!R->musicas[i])
        {
            return false;
        }

        else
        {
            printf("Musica cargada correctamente.\n");
        }
    }

    R->musica_actual = R->musicas[0];  // Música menú
    
    // Configurar volumen, balance, velocidad de reproducción
    al_set_sample_instance_gain(R->musica_actual->instancia, 1.0);  // Volumen
    al_set_sample_instance_pan(R->musica_actual->instancia, 0.0);  // Centro
    al_set_sample_instance_speed(R->musica_actual->instancia, 1.0);  // Velocidad 
    al_set_sample_instance_playmode(R->musica_actual->instancia, ALLEGRO_PLAYMODE_LOOP);  // Repetir

    // Reproducir
    al_play_sample_instance(R->musica_actual->instancia);

    R->sonido_rayo = cargar_efecto_sonido(SONIDO_RAYO, R->mixer);

    if (!R->sonido_rayo)
    {
        printf("Error al cargar el sonido del rayo.\n");
        return false;
    }

    inicializar_rayos(R->rayos, R->sonido_rayo);  // Inicializa los rayos de todos los niveles
    
    for (i=0; i<NRO_NIVELES; i++)
    {
        R->cantidad_rayos[i] = detectar_rayos(R->mapas[i], R->rayos[i], MAX_RAYOS);

    }

    return true;
}


/**
 * Función que inicializa todos los módulos y recursos de Allegro5 necesarios para el juego, además de inicializar al personaje principal.
 * @param R Puntero a la estructura Recursos donde se almacenarán los recursos creados.
 * @return true si se inicializaron todos los recursos correctamente, false en caso contrario.
 */
bool inicializar_todo(Recursos* R)
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

    return true;
}


/**
 * Función que libera todos los recursos utilizados por Allegro5.
 * Destruye la ventana, la cola de eventos y el temporizador.
 * @param R Puntero a la estructura Recursos donde se almacenan los recursos a liberar.
 */
Procedure finalizar_allegro(Recursos* R)
{
    Natural i;

    if (R->sonido_rayo != NULL)
    {
        for (i=0; i<NRO_INSTANCIAS; i++)
        {
            al_destroy_sample_instance(R->sonido_rayo->instancias[i]);
            R->sonido_rayo->instancias[i] = NULL;
        }
    }

    if (R->sonido_rayo != NULL)
    {
        al_destroy_sample(R->sonido_rayo->musica);
        R->sonido_rayo->musica = NULL;
    }

    free(R->sonido_rayo);
    R->sonido_rayo = NULL;

    for (i=0; i<NRO_MUSICAS; i++)
    {
        if (R->musicas[i])
        {
            if (R->musicas[i]->instancia != NULL)
            {
                al_destroy_sample_instance(R->musicas[i]->instancia);
                R->musicas[i]->instancia = NULL;
            }

            if (R->musicas[i]->musica != NULL)
            {
                al_destroy_sample(R->musicas[i]->musica);
                R->musicas[i]->musica = NULL;
            }

            free(R->musicas[i]);
            R->musicas[i] = NULL;
        }
    }

    if (R->mixer != NULL)
    {
        al_destroy_mixer(R->mixer);
        R->mixer = NULL;
    }

    if (R->voz != NULL)
    {
        al_destroy_voice(R->voz);
        R->voz = NULL;
    }

    // Se destruyen de forma iterativa todas las fuentes usadas
    for (i=0; i<NRO_FUENTES; i++)
    {
        if (R->fuentes[i] != NULL)
        {
            al_destroy_font(R->fuentes[i]);
            R->fuentes[i] = NULL;
        }
    }

    for (i=0; i<NRO_BLOQUES; i++)
    {
        if (R->bloques[i] != NULL)
        {
            al_destroy_bitmap(R->bloques[i]);
            R->bloques[i] = NULL;
        }
    }

    if (R->vida != NULL)
    {
        al_destroy_bitmap(R->vida);
        R->vida = NULL;
    }

    if (R->pje_principal.imagen_modo_muerte != NULL)
    {
        al_destroy_bitmap(R->pje_principal.imagen_modo_muerte);
        R->pje_principal.imagen_modo_muerte = NULL;
    }

    for (i=0; i<NRO_FRAMES; i++)
    {
        if (R->pje_principal.frames[i] != NULL)
        {
            al_destroy_bitmap(R->pje_principal.frames[i]);
            R->pje_principal.frames[i] = NULL;
        }
    }

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
}