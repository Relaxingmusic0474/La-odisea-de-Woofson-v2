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
 * Función que crea los recursos necesarios y más esenciales para el juego (propios de Allegro5).
 * Crea la ventana, la cola de eventos, el temporizador, y habilita algunos dispositivos.
 * @param R Puntero a la estructura Recursos donde se almacenarán los recursos creados.
 * @return true si se crearon los recursos de Allegro correctamente, false en caso contrario. 
 */ 
bool crear_recursos_allegro(Recursos* R)
{
    // Se crea la ventana
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

    // Se registran los eventos de la ventana en la cola de eventos
    al_register_event_source(R->cola_eventos, R->eventos.ventana);

    R->eventos.temporizador = al_get_timer_event_source(R->temporizador);

    if (!R->eventos.temporizador)
    {
        printf("No se pudieron obtener los eventos del temporizador.\n");
        return false;
    }

    // Se registran los eventos del temporizador en la cola de eventos
    al_register_event_source(R->cola_eventos, R->eventos.temporizador);

    R->eventos.teclado = al_get_keyboard_event_source();

    if (!R->eventos.teclado)
    {
        printf("No se pudieron obtener los eventos del teclado.\n");
        return false;
    }

    // Se registran los eventos del teclado en la cola de eventos
    al_register_event_source(R->cola_eventos, R->eventos.teclado);

    R->eventos.raton = al_get_mouse_event_source();

    if (!R->eventos.raton)
    {
        printf("No se pudieron obtener los eventos del mouse.\n");
        return false;
    }

    // Se registran los eventos del mouse en la cola de eventos
    al_register_event_source(R->cola_eventos, al_get_mouse_event_source());

    return true;
}


/**
 * Función que crea los recursos necesarios para el juego (tanto los recursos de Allegro como los que yo definí para el juego)
 * @param R Puntero a la estructura Recursos donde se almacenarán los recursos creados.
 * @return true si se crearon los recursos correctamente, false en caso contrario. 
 */ 
bool crear_recursos(Recursos* R)
{
    Natural i, j;
    char* textos_opciones_principal[] = {"JUGAR", "INSTRUCCIONES", "VER RANKING"};
    char* textos_opciones_niveles[] = {"1", "2", "3", "4", "5", "Volver atrás"};
    char* textos_opciones_ganar[] = {"Ir al menú principal", "Entrar en el ranking", "Ir al siguiente nivel"};
    char* textos_opciones_perder[] = {"Reintentar nivel", "Ir al menú principal"};
    char* textos_opciones_ranking[] = {"Volver atrás"};
    char ruta[40] = {'\0'};

    if (!crear_recursos_allegro(R))
    {
        return false;
    }

    if (!cargar_escenarios(R))
    {
        return false;
    }

    R->espina = al_load_bitmap("assets/images/espina.png");

    if (!R->espina)
    {
        printf("Error al cargar la imagen de la espina.\n");
        return false;
    }
    
    R->pje_principal.imagen_modo_muerte = al_load_bitmap("assets/images/muerte.png");

    if (!R->pje_principal.imagen_modo_muerte)
    {
        printf("Error al cargar la imagen modo muerte de Woofson.\n");
        return false;
    }
    
    if (!cargar_todos_los_frames(R->frames))  // Se cargan todos los frames
    {
        printf("Error al cargar los frames de los personajes.\n");
        return false;
    }
    
    // Se inicializa el personaje principal
    inicializar_personaje(&R->pje_principal, WOOFSON, R->frames, (Vector) {ANCHO_VENTANA*0.1, ALTURA_PISO-al_get_bitmap_height(R->frames[FRAME_WOOFSON][0])}, false);  

    R->vida = al_load_bitmap("assets/images/corazon.png");

    if (!R->vida)
    {
        printf("Error al cargar la imagen de las vidas del personaje.\n");
        return false;
    }

    for (i=0; i<NRO_BLOQUES; i++)
    {
        sprintf(ruta, "assets/images/bloque-%hu.png", i+1);
        R->bloques[i] = al_load_bitmap(ruta);

        if (!R->bloques[i])
        {
            printf("Error al cargar la imagen del %hu' bloque del juego.\n", i);
            return false;
        }

        memset(ruta, '\0', sizeof(ruta));
    }

    // Se cargan las imagenes de las palancas y las puertas
    for (i=0; i<NRO_ESTADOS; i++)
    {
        sprintf(ruta, "assets/images/palanca-%hu.png", i+1);
        R->palancas[i] = al_load_bitmap(ruta);

        if (!R->palancas[i])
        {
            printf("Error al cargar la imagen de la palanca %s del juego.\n", i==0 ? "desactivada" : "activada");
            return false;
        }

        memset(ruta, '\0', sizeof(ruta));

        sprintf(ruta, "assets/images/puerta-%hu.png", i+1);
        R->puertas[i] = al_load_bitmap(ruta);

        if (!R->puertas[i])
        {
            printf("Error al cargar la imagen de la puerta %s del juego.\n", i==0 ? "cerrada" : "abierta");
            return false;
        }

        memset(ruta, '\0', sizeof(ruta));
    }

    R->pocion = al_load_bitmap("assets/images/pocion.png");

    if (!R->pocion)
    {
        printf("Error al cargar la imagen de la pocion\n");
        return false;
    }

    for (i=0; i<MAX_POCIONES; i++)
    {
        R->pociones[i].imagen = R->pocion;
        R->pociones[i].tomada = false;  // Aun no es tomada por Woofson
    }

    R->pocion_rango_bala = al_load_bitmap("assets/images/pocion-2.png");

    if (!R->pocion_rango_bala)
    {
        printf("Error al cargar la imagen de la pocion que aumenta el rango de bala\n");
        return false;
    }

    for (i=0; i<MAX_POCIONES_RANGO_BALA; i++)
    {
        R->pociones_rango_bala[i].imagen = R->pocion_rango_bala;
        R->pociones_rango_bala[i].tomada = false;   // Aun no es tomada por Woofson
    }

    R->municion = al_load_bitmap("assets/images/balas.png");

    if (!R->municion)
    {
        printf("Error al cargar la imagen de las municiones normales\n");
        return false;
    }

    R->municion_pro = al_load_bitmap("assets/images/balas-2.png");
    
    if (!R->municion_pro)
    {
        printf("Error al cargar la imagen de las municiones pro\n");
        return false;
    }

    for (i=0; i<MAX_MUNICIONES; i++)
    {
        R->municiones[i].imagen = R->municion;
        R->municiones[i].tomada = false;
    }

    R->fuego = al_load_bitmap("assets/images/fuego.png");

    if (!R->fuego)
    {
        printf("Error al cargar la imagen del fuego\n");
        return false;
    }

    // Se carga la imagen de fondo del menú principal
    R->fondo_menu_principal = al_load_bitmap("assets/images/menu.jpg");

    if (!R->fondo_menu_principal)
    {
        printf("Error al cargar el fondo del menú principal\n");
        return false;
    }

    // Se carga la imagen de fondo del menú de niveles
    R->fondo_menu_niveles = al_load_bitmap("assets/images/menu_niveles.png");

    if (!R->fondo_menu_niveles)
    {
        printf("Error al cargar el fondo del menú de niveles\n");
        return false;
    }

    // Se cargan las fuentes
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
        }
    }

    if (!inicializar_menu(&R->menus[PRINCIPAL], PRINCIPAL, R->fondo_menu_principal, R->fuentes[TIMES_NEW_ROMAN_GRANDE], NULL, 
                          textos_opciones_principal, NRO_ELEMS(textos_opciones_principal), RECTANGULO_VENTANA))
    {
        return false;
    }
    
    if (!inicializar_menu(&R->menus[NIVELES], NIVELES, R->fondo_menu_niveles, R->fuentes[COMFORTAA_LIGHT_GRANDE], NULL, 
                          textos_opciones_niveles, NRO_ELEMS(textos_opciones_niveles), RECTANGULO_VENTANA))
    {
        return false;
    }
    
    if (!inicializar_menu(&R->menus[PERDER], PERDER, NULL, R->fuentes[TIMES_NEW_ROMAN_GIGANTE], R->fuentes[TIMES_NEW_ROMAN_GRANDE], 
                          textos_opciones_perder, NRO_ELEMS(textos_opciones_perder), RECTANGULO_MENU_RESULTADO))
    {
        return false;
    }
    
    if (!inicializar_menu(&R->menus[GANAR], GANAR, NULL, R->fuentes[TIMES_NEW_ROMAN_GIGANTE], R->fuentes[TIMES_NEW_ROMAN_GRANDE],
                          textos_opciones_ganar, NRO_ELEMS(textos_opciones_ganar), RECTANGULO_MENU_RESULTADO))
    {
        return false;
    }

    if (!inicializar_menu(&R->menus[MENU_RANK], MENU_RANK, NULL, R->fuentes[COMFORTAA_LIGHT_GIGANTE], R->fuentes[COMFORTAA_LIGHT_GRANDE], 
                          textos_opciones_ranking, NRO_ELEMS(textos_opciones_ranking), RECTANGULO_VENTANA))
    {
        return false;
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

    // Se cargan las músicas
    for (i=0; i<NRO_MUSICAS; i++)
    {
        R->musicas[i] = i==0 ? cargar_musica(MUSICA_MENU, R->mixer) : (i==1 ? cargar_musica(MUSICA_NIVEL_1, R->mixer) : cargar_musica(MUSICA_NIVEL_4, R->mixer));
        
        if (!R->musicas[i])
        {
            return false;
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

    // Se carga el efecto de sonido de los rayos
    R->sonido_rayo = cargar_efecto_sonido(SONIDO_RAYO, R->mixer);

    if (!R->sonido_rayo)
    {
        printf("Error al cargar el sonido del rayo.\n");
        return false;
    }

    inicializar_rayos(R->rayos, R->sonido_rayo);  // Inicializa los rayos de todos los niveles
    
    for (i=0; i<NRO_NIVELES; i++)  // Se detecta la cantidad de rayos que hay en cada nivel, además de dónde están
    {
        R->cantidad_rayos[i] = detectar_rayos(R->mapas[i], R->rayos[i], MAX_RAYOS);
    }

    for (i=0; i<NRO_NIVELES; i++)
    {
        if (!leer_ranking(&R->rankings[i], i+1))
        {
            return false;
        }
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
    // Se inicializa la semilla para los números aleatorios
    srand(time(NULL));

    // Se inicializan todos los módulos de allegro
    if (!inicializar_allegro())
    {
        finalizar_allegro(R);  // Por si el error ocurre después de haber inicializado algunos módulos
        return false;
    }

    // Se crean los recursos necesarios para el juego (ventana, cola de eventos, temporizador, mapa, etc)
    if (!crear_recursos(R))
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

    // Se destruye el mixer usado
    if (R->mixer != NULL)
    {
        al_destroy_mixer(R->mixer);
        R->mixer = NULL;
    }

    // Se destruye la voz usada
    if (R->voz != NULL)
    {
        al_destroy_voice(R->voz);
        R->voz = NULL;
    }

    // Se finalizan los menús
    for (i=0; i<NRO_MENUS; i++)
    {
        if (!R->menus[i].finalizado)
        {
            finalizar_menu(&R->menus[i]);
        }
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

    if (R->fondo_menu_niveles != NULL)
    {
        al_destroy_bitmap(R->fondo_menu_niveles);
        R->fondo_menu_niveles = NULL;
    }

    if (R->fondo_menu_principal != NULL)
    {
        al_destroy_bitmap(R->fondo_menu_principal);
        R->fondo_menu_principal = NULL;
    }

    if (R->fuego != NULL)
    {
        al_destroy_bitmap(R->fuego);
        R->fuego = NULL;
    }

    if (R->municion_pro != NULL)
    {
        al_destroy_bitmap(R->municion_pro);
        R->municion_pro = NULL;
    }

    if (R->municion != NULL)
    {
        al_destroy_bitmap(R->municion);
        R->municion = NULL;
    }

    if (R->pocion_rango_bala != NULL)
    {
        al_destroy_bitmap(R->pocion_rango_bala);
        R->pocion_rango_bala = NULL;
    }

    if (R->pocion != NULL)
    {
        al_destroy_bitmap(R->pocion);
        R->pocion = NULL;
    }

    // Se destruyen las palancas y las puertas
    for (i=0; i<NRO_ESTADOS; i++)
    {
        if (R->palancas[i] != NULL)
        {
            al_destroy_bitmap(R->palancas[i]);
            R->palancas[i] = NULL;
        }

        if (R->puertas[i] != NULL)
        {
            al_destroy_bitmap(R->puertas[i]);
            R->puertas[i] = NULL;
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
    
    destruir_todos_los_frames(R->frames);

    if (R->espina != NULL)
    {
        al_destroy_bitmap(R->espina);
        R->espina = NULL;
    }

    liberar_mapas(R->mapas);

    for (i=0; i<NRO_NIVELES; i++)
    {
        if (R->fondos[i] != NULL)
        {
            al_destroy_bitmap(R->fondos[i]);
            R->fondos[i] = NULL;
        }
    }

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