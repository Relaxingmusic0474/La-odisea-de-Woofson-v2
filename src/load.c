#include "load.h"

ALLEGRO_FONT* cargar_fuente(Fuente tipo_fuente, Natural tamanho)  /* Carga la fuente */
{
    ALLEGRO_FONT* fuente = NULL;

    switch (tipo_fuente)
    {
        case COMFORTAA_LIGHT:
            fuente = al_load_font("assets/fonts/Comfortaa-Light.ttf", tamanho, 0);
            break;

        case TIMES_NEW_ROMAN:
            fuente = al_load_font("assets/fonts/times.ttf", tamanho, 0);
            break;

        default:
            printf("Tipo de fuente no valido.\n");
            return NULL;
    }

    if (!fuente)
    {
        printf("Error al cargar la fuente.\n");
    }

    return fuente;
}


Musica* cargar_musica(TipoAudio tipo, ALLEGRO_MIXER* mixer)
{
    Musica* musica = (Musica*) malloc(sizeof(Musica));

    if (!musica)
    {
        printf("Error de asignacion de memoria al cargar la musica.\n");
        return NULL;
    }

    // Se inicializa la estructura de la música
    musica->tipo = tipo;
    musica->musica = NULL;  // El sample de la música
    musica->instancia = NULL;

    switch (musica->tipo)
    {
        case MUSICA_MENU:
            musica->musica = al_load_sample("assets/music/Soundtrack-menu.ogg");
            break;

        case MUSICA_NIVEL_1:
        case MUSICA_NIVEL_2:
        case MUSICA_NIVEL_3:
            musica->musica = al_load_sample("assets/music/Soundtrack-A.ogg");  // Se usa el mismo sample para los niveles 1, 2 y 3
            break;

        case MUSICA_NIVEL_4:
        case MUSICA_NIVEL_5:
            musica->musica = al_load_sample("assets/music/Soundtrack-B.ogg");  // Se usa el mismo sample para los niveles 4 y 5
            break;

        default:
            printf("Tipo de audio no valido.\n");
            return NULL;
    }

    if (!musica->musica)
    {
        if (tipo == MUSICA_MENU)
        {
            printf("Error al cargar el sample \"Soundtrack-menu.ogg\".\n");
        }

        else if (tipo == MUSICA_NIVEL_1 || tipo == MUSICA_NIVEL_2 || tipo == MUSICA_NIVEL_3)
        {
            printf("Error al cargar el sample \"Soundtrack-A.ogg\".\n");
        }

        else if (tipo == MUSICA_NIVEL_4 || tipo == MUSICA_NIVEL_5)
        {
            printf("Error al cargar el sample \"Soundtrack-B.ogg\".\n");
        }

        else
        {
            printf("Error al cargar el sample \"Rayo.ogg\".\n");
        }
        
        free(musica);
        musica = NULL;
        return NULL;
    }

    musica->instancia = al_create_sample_instance(musica->musica);

    if (!musica->instancia)
    {
        printf("Error al crear instancia.\n");
        al_destroy_sample(musica->musica);
        musica->musica = NULL;
        free(musica);
        musica = NULL;
        return NULL;
    }

    al_attach_sample_instance_to_mixer(musica->instancia, mixer);

    return musica;
}


/** 
 * Función que carga un efecto de sonido con sus instancias correspondientes.
 * @param tipo Es el tipo de efecto de sonido que se cargará.
 * @param mixer Es el mixer que se conectará a las instancias de sonido.
 * @return El efecto de su sonido con sus instancias correspondientes.
 */
EfectoSonido* cargar_efecto_sonido(TipoEfecto tipo, ALLEGRO_MIXER* mixer)
{
    Natural i, j;
    Audio audio;
    EfectoSonido* efecto = (EfectoSonido*) malloc(sizeof(EfectoSonido));

    if (!efecto)
    {
        printf("Error de asignacion de memoria al cargar la musica.\n");
        return NULL;
    }

    efecto->tipo = tipo;

    switch (tipo)
    {
        case SONIDO_RAYO:
            audio = al_load_sample("assets/music/Rayo.ogg");
            break;

        default:
            printf("Tipo de efecto no válido\n");
            return NULL;
    }

    if (!audio)
    {
        printf("Error al cargar el sample de efecto \"Rayo.ogg\"\n");
        free(efecto);
        return NULL;
    }

    efecto->musica = audio;

    for (i=0; i<NRO_INSTANCIAS; i++)
    {
        efecto->instancias[i] = al_create_sample_instance(audio);

        if (!efecto->instancias[i])
        {
            printf("Error al crear una de las instancias del efecto.\n");

            for (j=0; j<i; j++)
            {    
                al_destroy_sample_instance(efecto->instancias[i]);
                efecto->instancias[i] = NULL;
            }

            al_destroy_sample(audio);
            audio = NULL;
            efecto->musica = NULL;
            free(efecto);
            efecto = NULL;
            return NULL;
        }

        al_attach_sample_instance_to_mixer(efecto->instancias[i], mixer);
    }

    return efecto;
}


/**
 * Función para cambiar la música actual.
 * @param R Puntero a la estructura Recursos (estructura global del juego).
 * @param musica_nueva La música que se desea reproducir.
 * @return true si cambia la música correctamente, y false en caso contrario.
 */
bool cambiar_musica(Recursos* R, Musica* musica_nueva)
{
    // Se para la musica actual y se cambia la musica actual
    if (!al_stop_sample_instance(R->musica_actual->instancia))
    {
        return false;
    }

    R->musica_actual = musica_nueva;

    if (!al_get_sample_instance_attached(R->musica_actual->instancia)) 
    {
        if (!al_attach_sample_instance_to_mixer(R->musica_actual->instancia, R->mixer)) 
        {
            printf("ERROR: No se pudo conectar la instancia al mixer\n");
            return false;
        }
    }

    // Configurar volumen, balance, velocidad de reproducción
    al_set_sample_instance_gain(R->musica_actual->instancia, 0.5);  // Volumen
    al_set_sample_instance_pan(R->musica_actual->instancia, 0.0);  // Centro
    al_set_sample_instance_speed(R->musica_actual->instancia, 1.0);  // Velocidad normal
    al_set_sample_instance_playmode(R->musica_actual->instancia, ALLEGRO_PLAYMODE_LOOP);  // Repetir
            
    // Reproducir
    al_play_sample_instance(R->musica_actual->instancia);

    return true;
}


/**
 * Función para cargar el mapa.
 * @param nro_nivel Es el nivel del cual se desea cargar su mapa.
 * @return El puntero al archivo del mapa.
 */
FILE* cargar_mapa(Natural nro_nivel)
{
    FILE* archivo_mapa = NULL;
    char ruta[LARGO] = {'\0'};

    snprintf(ruta, sizeof(ruta), "assets/maps/nivel%hu.txt", nro_nivel);

    archivo_mapa = fopen(ruta, "r");

    if (!archivo_mapa)
    {
        printf("Error al cargar el archivo del mapa %s\n", ruta);
        return NULL;
    }

    return archivo_mapa;
}


/**
 * Función que devuelve el número de filas de un mapa de cierto nivel del juego.
 * @param nro_nivel Es el número de nivel del cual se desea obtener su cantidad de filas.
 * @return El número de filas del mapa que leyó.
 */
Natural obtener_nro_filas(Natural nro_nivel)
{
    Natural filas = 0;
    char linea[MAXLINEA] = {'\0'};

    FILE* archivo_mapa = cargar_mapa(nro_nivel);

    if (!archivo_mapa)
    {
        return 0;
    }

    while (fgets(linea, MAXLINEA, archivo_mapa))
    {
        filas++;
    }
    
    fclose(archivo_mapa);

    return filas;
}


/**
 * Función que devuelve el número de columnas de un mapa de cierto nivel del juego.
 * @param nro_nivel Es el número de nivel del cual se desea obtener su cantidad de columnas.
 * @return El número de columnas del mapa que leyó.
 */
Natural obtener_nro_columnas(Natural nro_nivel)
{
    Natural columnas = 0;
    char linea[MAXLINEA] = {'\0'};
    char* token = NULL;
    FILE* archivo_mapa = cargar_mapa(nro_nivel);

    if (!archivo_mapa)
    {
        return 0;
    }

    if (fgets(linea, MAXLINEA, archivo_mapa))
    {
        token = strtok(linea, " \n\r");

        while (token)
        {
            columnas++;
            token = strtok(NULL, " \n\r");
        }
    }

    fclose(archivo_mapa);

    return columnas;
}


/**
 * Función que leerá el mapa de un cierto nivel, devolviendo la matriz.
 * @param nro_nivel El nivel del cual se puede cargar un mapa.
 * @return La matriz que se lee en el mapa.
 */
Mapa leer_mapa(Natural nro_nivel/*, Natural* nro_filas, Natural* nro_columnas*/)
{
    Natural i=0, j=0;
    Mapa mapa = MAPA_INVALIDO;
    FILE* archivo_mapa = NULL;

    mapa.nro_filas = obtener_nro_filas(nro_nivel);

    if (mapa.nro_filas == 0)
    {
        return mapa;
    }

    mapa.nro_columnas = obtener_nro_columnas(nro_nivel);

    if (mapa.nro_columnas == 0)
    {
        return MAPA_INVALIDO;
    }

    mapa.mapa = (Natural **) malloc(mapa.nro_filas * sizeof(Natural*));

    if (!mapa.mapa)
    {
        printf("Error de asignación de memoria");
        return MAPA_INVALIDO;
    }

    for (i=0; i<mapa.nro_filas; i++)
    {
        mapa.mapa[i] = (Natural *) malloc(mapa.nro_columnas * sizeof(Natural));

        if (!mapa.mapa[i])
        {
            printf("Error de asignación de memoria");
            liberar_mapa(&mapa);  // Liberar el mapa ya creado hasta ahora
            return MAPA_INVALIDO;
        }
    }

    archivo_mapa = cargar_mapa(nro_nivel);

    if (!archivo_mapa)
    {
        liberar_mapa(&mapa);
        return MAPA_INVALIDO;
    }

    i=0, j=0;

    /* Como ya se sabe el nro de filas y el nro de columnas que tiene el mapa, ahora se lee cada uno de sus elementos */
    while (fscanf(archivo_mapa, "%hu ", &mapa.mapa[i][j]) == 1)
    {
        j = (j+1) % (mapa.nro_columnas);
        
        if (j == 0)
        {
            i++;
        }
    }

    /* Como ya se leyó el mapa, se cierra el archivo */
    fclose(archivo_mapa);

    mapa.alto_bloque = (float) ALTURA_PISO / mapa.nro_filas;
    mapa.ancho_bloque = (float) ANCHO_VENTANA / mapa.nro_columnas;

    return mapa;
}


/**
 * Función que dibuja el mapa en la ventana.
 * @param mapa Es el mapa que se desea dibujar.
 */
Procedure dibujar_mapa(Mapa mapa, Recursos* recursos, bool* cambio_estado_procesado, Natural iteracion)
{
    Natural i=0, j=0, k=0;
    Natural id_enemigo = 0, id_pocion = 0, id_municion = 0, id_pocion_rango_bala = 0;
    Entero flag = 0;
    Personaje* woofson = &recursos->pje_principal;
    Imagen bloque_cafe = recursos->bloques[0];
    Imagen bloque_plateado = recursos->bloques[1];
    extern bool teclas[ALLEGRO_KEY_MAX];
    bool aux = false;
    float ancho_espina, alto_espina, x1, y1, x2, y2;
    float ancho_esc, alto_esc;
    float alto_enemigo;
    
    ancho_espina = al_get_bitmap_width(recursos->espina);
    alto_espina = al_get_bitmap_height(recursos->espina);

    ancho_esc = ancho_espina * FACTOR_ESPINA;
    alto_esc = alto_espina * FACTOR_ESPINA;
    
    for (i=0; i<mapa.nro_filas; i++)
    {
        for (j=0; j<mapa.nro_columnas; j++)
        {
            x1 = j*mapa.ancho_bloque;
            y1 = i*mapa.alto_bloque;

            x2 = (j+1)*mapa.ancho_bloque;
            y2 = (i+1)*mapa.alto_bloque;

            if (mapa.mapa[i][j] == BLOQUE || mapa.mapa[i][j] == BLOQUE_RAYO)
            {
                al_draw_scaled_bitmap(bloque_plateado, 0, 0, al_get_bitmap_width(bloque_plateado), al_get_bitmap_height(bloque_plateado), x1, y1, mapa.ancho_bloque, mapa.alto_bloque, 0);
            }

            if (mapa.mapa[i][j] == BLOQUE_RAYO)
            {
                al_draw_filled_circle((x1+x2)/2, (y1+y2)/2, RADIO_CIRCULO_ROJO, ROJO);
            }

            if (mapa.mapa[i][j] == ESPINA)
            {
                if (i == mapa.nro_filas-1 || (i < mapa.nro_filas-1 && mapa.mapa[i+1][j] == BLOQUE))
                {
                    flag = 0;
                    aux = true;
                }

                else if (i == 0 || (i > 0 && mapa.mapa[i-1][j] == BLOQUE))
                {
                    flag = ALLEGRO_FLIP_VERTICAL;
                    aux = true;
                }

                else
                {
                    if (j == 0 || (j > 0 && mapa.mapa[i][j-1] == BLOQUE))
                    {
                        al_draw_tinted_scaled_rotated_bitmap(recursos->espina, ROJO, ancho_espina/2, alto_espina/2, x1+alto_esc/2, (y1+y2)/2, 
                                                             FACTOR_ESPINA, FACTOR_ESPINA, ALLEGRO_PI/2, 0);
                    }

                    else
                    {
                        if (j == mapa.nro_columnas-1 || (j < mapa.nro_columnas-1 && mapa.mapa[i][j+1] == BLOQUE))
                        {
                            al_draw_tinted_scaled_rotated_bitmap(recursos->espina, ROJO, ancho_espina/2, alto_espina/2, x2-alto_esc/2, 
                                                                 (y1+y2)/2, FACTOR_ESPINA, FACTOR_ESPINA, -ALLEGRO_PI/2, 0);
                        }
                    }

                    aux = false;
                }

                if (aux)
                {
                    al_draw_tinted_scaled_bitmap(recursos->espina, ROJO, 0, 0, ancho_espina, alto_espina, 
                                                 (x1+x2-ancho_esc)/2, y1+(mapa.alto_bloque-alto_esc)*(ALLEGRO_FLIP_VERTICAL-flag)/ALLEGRO_FLIP_VERTICAL, 
                                                 ancho_esc, alto_esc, flag);
                }
            }

            if (mapa.mapa[i][j] == EXTRATERRESTRE_ESTATICO || mapa.mapa[i][j] == EXTRATERRESTRE_DINAMICO || mapa.mapa[i][j] == _MONSTRUO)
            {
                if (i==mapa.nro_filas-1 || (i<mapa.nro_filas-1 && (mapa.mapa[i+1][j] == BLOQUE || mapa.mapa[i+1][j] == BLOQUE_RAYO)))
                {
                    if  (id_enemigo < MAX_ENEMIGOS)
                    {
                        if (!recursos->enemigos[id_enemigo].inicializado)
                        {
                            alto_enemigo = al_get_bitmap_height(recursos->frames[(TipoFrame) (1./6 * pow(mapa.mapa[i][j], 2) - 1.5 * mapa.mapa[i][j] + 16./3)][0]);  // Polinomio de interpolación de Lagrange

                            recursos->enemigos[id_enemigo].posicion_inicial.x = x1;
                            recursos->enemigos[id_enemigo].posicion_inicial.y = y2 - alto_enemigo - 1;

                            inicializar_personaje(&recursos->enemigos[id_enemigo], mapa.mapa[i][j] <= EXTRATERRESTRE_DINAMICO ? EXTRATERRESTRE : MONSTRUO, recursos->frames, 
                                                  recursos->enemigos[id_enemigo].posicion_inicial, mapa.mapa[i][j] == EXTRATERRESTRE_ESTATICO ? true : false);
                        }                        

                        else  // Si ya estaba inicializado
                        {
                            if (recursos->enemigos[id_enemigo].estatico)
                            {
                                if (woofson->posicion.x + woofson->ancho < recursos->enemigos[id_enemigo].posicion.x)
                                {
                                    recursos->enemigos[id_enemigo].bandera_dibujo = ALLEGRO_FLIP_HORIZONTAL;
                                    recursos->enemigos[id_enemigo].direccion = -1;
                                }

                                else
                                {
                                    if (woofson->posicion.x > recursos->enemigos[id_enemigo].posicion.x + recursos->enemigos[id_enemigo].ancho)
                                    {
                                        recursos->enemigos[id_enemigo].bandera_dibujo = 0;
                                        recursos->enemigos[id_enemigo].direccion = 1;
                                    }
                                }
                            }

                            else  // Si es dinámico
                            {
                                if (recursos->enemigos[id_enemigo].direccion == 1)
                                {
                                    recursos->enemigos[id_enemigo].bandera_dibujo = 0;
                                }

                                else
                                {
                                    recursos->enemigos[id_enemigo].bandera_dibujo = ALLEGRO_FLIP_HORIZONTAL;
                                }

                                mover_enemigo_dinamico(&recursos->enemigos[id_enemigo], *woofson, mapa);
                            }

                            if (!recursos->enemigos[id_enemigo].muerto)
                            {   
                                dibujar_personaje(&recursos->enemigos[id_enemigo], 0, iteracion);
                            }
                        }
                    }
                    
                    id_enemigo++;
                }
            }

            if (mapa.mapa[i][j] == _DRAGON)
            {
                if (id_enemigo < MAX_ENEMIGOS)
                {
                    if (!recursos->enemigos[id_enemigo].inicializado)
                    {
                        alto_enemigo = 0.7 * al_get_bitmap_height(recursos->frames[FRAME_DRAGON][0]);

                        recursos->enemigos[id_enemigo].posicion_inicial.x = mapa.ancho_bloque*j;
                        recursos->enemigos[id_enemigo].posicion_inicial.y = mapa.alto_bloque*(i+1) - alto_enemigo - 1;

                        inicializar_personaje(&recursos->enemigos[id_enemigo], DRAGON, recursos->frames, 
                                              recursos->enemigos[id_enemigo].posicion_inicial, false);
                    }

                    else
                    {
                        if (j > 0.5*mapa.nro_columnas)
                        {
                            recursos->enemigos[id_enemigo].direccion = -1;
                            recursos->enemigos[id_enemigo].bandera_dibujo = 0;
                        }

                        else
                        {
                            recursos->enemigos[id_enemigo].direccion = 1;
                            recursos->enemigos[id_enemigo].bandera_dibujo = ALLEGRO_FLIP_HORIZONTAL;
                        }

                        mover_enemigo_dinamico(&recursos->enemigos[id_enemigo], *woofson, mapa);
                    }

                    if (!recursos->enemigos[id_enemigo].muerto)
                    {
                        dibujar_personaje(&recursos->enemigos[id_enemigo], 0, iteracion);
                    }
                }

                id_enemigo++;
            }

            if (mapa.mapa[i][j] == PUERTA)
            {
                if (recursos->palanca.estado == DESACTIVADA)
                {
                    recursos->puerta.imagen = recursos->puertas[CERRADA];
                    recursos->puerta.estado = CERRADA;
                }

                else
                {
                    recursos->puerta.imagen = recursos->puertas[ABIERTA];
                    recursos->puerta.estado = ABIERTA;
                }

                recursos->puerta.alto = al_get_bitmap_height(recursos->puerta.imagen);
                recursos->puerta.ancho = al_get_bitmap_width(recursos->puerta.imagen);
                recursos->puerta.posicion = (Vector) {x1, y2-recursos->puerta.alto+15};
                al_draw_bitmap(recursos->puerta.imagen, recursos->puerta.posicion.x, recursos->puerta.posicion.y, j>mapa.nro_columnas/2 ? 0 : ALLEGRO_FLIP_HORIZONTAL);
            }

            if (mapa.mapa[i][j] == PALANCA)
            {
                recursos->palanca.imagen = recursos->palancas[DESACTIVADA];
                recursos->palanca.posicion = (Vector) {x1, y2-recursos->palanca.alto};
                recursos->palanca.alto = al_get_bitmap_height(recursos->palanca.imagen);
                recursos->palanca.ancho = al_get_bitmap_width(recursos->palanca.imagen);

                if (woofson->posicion.x + woofson->ancho >= recursos->palanca.posicion.x && woofson->posicion.y + woofson->alto >= recursos->palanca.posicion.y &&
                    woofson->posicion.x < recursos->palanca.posicion.x + recursos->palanca.ancho && woofson->posicion.y < recursos->palanca.posicion.y + recursos->palanca.alto)
                {
                    if (teclas[ALLEGRO_KEY_E])
                    {
                        if (!(*cambio_estado_procesado))
                        {
                            *cambio_estado_procesado = true;
                            recursos->palanca.estado = recursos->palanca.estado == ACTIVADA ? DESACTIVADA : ACTIVADA;
                            recursos->palanca.imagen = recursos->palancas[recursos->palanca.estado];
                            recursos->palanca.alto = al_get_bitmap_height(recursos->palanca.imagen);
                            recursos->palanca.ancho = al_get_bitmap_width(recursos->palanca.imagen);
                        }
                    }

                    else
                    {
                        *cambio_estado_procesado = false;
                    }
                }

                recursos->palanca.imagen = recursos->palancas[recursos->palanca.estado];
                al_draw_bitmap(recursos->palanca.imagen, recursos->palanca.posicion.x, recursos->palanca.posicion.y, 0);
            }

            if (mapa.mapa[i][j] == POCION)
            {
                if (i==mapa.nro_filas-1 || (i<mapa.nro_filas-1 && (mapa.mapa[i+1][j] == BLOQUE || mapa.mapa[i+1][j] == BLOQUE_RAYO)))
                {
                    if (id_pocion < MAX_POCIONES)
                    {
                        if (!recursos->pociones[id_pocion].tomada)  // Si aun no ha sido tomada se dibuja
                        {
                            al_draw_bitmap(recursos->pociones[id_pocion].imagen, (x1+x2-al_get_bitmap_width(recursos->pociones[id_pocion].imagen))/2, 
                                           y2-al_get_bitmap_height(recursos->pociones[id_pocion].imagen), 0);

                            if (woofson->posicion.x + woofson->ancho >= (x1+x2-al_get_bitmap_width(recursos->pociones[id_pocion].imagen))/2 && 
                                woofson->posicion.y + woofson->alto >= y2-al_get_bitmap_height(recursos->pociones[id_pocion].imagen) &&
                                woofson->posicion.x < (x1+x2+al_get_bitmap_width(recursos->pociones[id_pocion].imagen))/2 &&
                                woofson->posicion.y < y2)
                            {
                                if (teclas[ALLEGRO_KEY_E])
                                {
                                    recursos->pociones[id_pocion].tomada = true;
                                    woofson->nro_vidas++;
                                }
                            }
                        }
                    }
                }

                id_pocion++;
            }

            if (mapa.mapa[i][j] == MUNICION)
            {
                if (i==mapa.nro_filas-1 || (i<mapa.nro_filas-1 && (mapa.mapa[i+1][j] == BLOQUE || mapa.mapa[i+1][j] == BLOQUE_RAYO)))
                {
                    if (id_municion < MAX_MUNICIONES)
                    {
                        if (!recursos->municiones[id_municion].tomada)
                        {
                            al_draw_bitmap(recursos->municiones[id_municion].imagen, (x1+x2-al_get_bitmap_width(recursos->municiones[id_municion].imagen))/2, 
                                           y2-al_get_bitmap_height(recursos->municiones[id_municion].imagen), 0);

                            if (woofson->posicion.x + woofson->ancho >= (x1+x2-al_get_bitmap_width(recursos->municiones[id_municion].imagen))/2 && 
                                woofson->posicion.y + woofson->alto >= y2-al_get_bitmap_height(recursos->municiones[id_municion].imagen) &&
                                woofson->posicion.x < (x1+x2+al_get_bitmap_width(recursos->municiones[id_municion].imagen))/2 &&
                                woofson->posicion.y < y2 && woofson->balas_disponibles == 0 && woofson->bala_recargable == false)
                            {
                                if (teclas[ALLEGRO_KEY_E])
                                {
                                    for (k=0; k<MAX_BALAS; k++)
                                    {
                                        woofson->balas[k].disponible = true;
                                        woofson->balas[k].frames_para_disponibilidad = 0;
                                        recursos->municiones[id_municion].tomada = true;
                                    }
                                }
                            }
                        }
                    }
                }

                id_municion++;
            }
        
            if (mapa.mapa[i][j] == POCION_RANGO_BALA)
            {
                if (i==mapa.nro_filas-1 || (i<mapa.nro_filas-1 && (mapa.mapa[i+1][j] == BLOQUE || mapa.mapa[i+1][j] == BLOQUE_RAYO)))
                {
                    if (id_pocion_rango_bala < MAX_POCIONES_RANGO_BALA)
                    {
                        if (!recursos->pociones_rango_bala[id_pocion_rango_bala].tomada)  // Si aun no ha sido tomada se dibuja
                        {
                            al_draw_bitmap(recursos->pociones_rango_bala[id_pocion_rango_bala].imagen, 
                                          (x1+x2-al_get_bitmap_width(recursos->pociones_rango_bala[id_pocion_rango_bala].imagen))/2, 
                                           y2-al_get_bitmap_height(recursos->pociones_rango_bala[id_pocion_rango_bala].imagen), 0);

                            if (woofson->posicion.x + woofson->ancho >= (x1+x2-al_get_bitmap_width(recursos->pociones_rango_bala[id_pocion_rango_bala].imagen))/2 && 
                                woofson->posicion.y + woofson->alto >= y2-al_get_bitmap_height(recursos->pociones_rango_bala[id_pocion_rango_bala].imagen) &&
                                woofson->posicion.x < (x1+x2+al_get_bitmap_width(recursos->pociones_rango_bala[id_pocion_rango_bala].imagen))/2 &&
                                woofson->posicion.y < y2)
                            {
                                if (teclas[ALLEGRO_KEY_E])
                                {
                                    recursos->pociones_rango_bala[id_pocion_rango_bala].tomada = true;
                                    woofson->bala_maximo_alcance = true;
                                    woofson->tiempo_restante_bala_maximo_alcance = MAX_TIEMPO_BALA_MAXIMO_ALCANCE;
                                }
                            }
                        }
                    }

                    id_pocion_rango_bala++;
                }
            }
         
            if (mapa.mapa[i][j] == DUENDE)
            {
                if (i==mapa.nro_filas-1 || (i<mapa.nro_filas-1 && (mapa.mapa[i+1][j] == BLOQUE || mapa.mapa[i+1][j] == BLOQUE_RAYO)))
                {
                    if  (id_enemigo < MAX_ENEMIGOS)
                    {
                        if (!recursos->enemigos[id_enemigo].inicializado)
                        {
                            alto_enemigo = al_get_bitmap_height(recursos->frames[FRAME_LEPRECHAUN][0]);  // Polinomio de interpolación de Lagrange

                            recursos->enemigos[id_enemigo].posicion_inicial.x = x1;
                            recursos->enemigos[id_enemigo].posicion_inicial.y = y2 - alto_enemigo - 1;

                            inicializar_personaje(&recursos->enemigos[id_enemigo], LEPRECHAUN, recursos->frames, 
                                                  recursos->enemigos[id_enemigo].posicion_inicial, false);
                        }                        

                        else  // Si ya estaba inicializado
                        {                            
                            if (recursos->enemigos[id_enemigo].direccion == 1)
                            {
                                recursos->enemigos[id_enemigo].bandera_dibujo = ALLEGRO_FLIP_HORIZONTAL;
                            }

                            else
                            {
                                recursos->enemigos[id_enemigo].bandera_dibujo = 0;
                            }

                            mover_enemigo_dinamico(&recursos->enemigos[id_enemigo], *woofson, mapa);

                            if (!recursos->enemigos[id_enemigo].muerto)
                            {   
                                dibujar_personaje(&recursos->enemigos[id_enemigo], 0, iteracion);
                            }
                        }
                    }

                    id_enemigo++;
                }
            }
        }
    }

    for (i=0; i<mapa.nro_columnas; i++)
    {
        al_draw_bitmap(bloque_cafe, i*mapa.ancho_bloque, ALTURA_PISO, 0);
    }

    return;
}


/**
 * Función que libera la memoria de un mapa.
 * @param mapa Es el mapa a liberar.
 */
Procedure liberar_mapa(Mapa* mapa)
{
    Natural i=0;

    for (i=0; i<mapa->nro_filas; i++)
    {
        free(mapa->mapa[i]);
        mapa->mapa[i] = NULL;        
    }

    free(mapa->mapa);
    mapa->mapa = NULL;

    /* Reinicia el mapa a su estado inicial */
    mapa->nro_filas = 0;
    mapa->nro_columnas = 0;

    return;
}


/**
 * Función que libera todos los mapas del juego (todos los mapas de los niveles).
 * @param mapas Es el arreglo de mapas a liberar.
 */
Procedure liberar_mapas(Mapa mapas[])
{
    Natural i=0;

    for (i=0; i<NRO_NIVELES; i++)
    {
        if (mapas[i].mapa != NULL)
        {
            liberar_mapa(&mapas[i]);
        }
    }

    return;
}


bool cargar_escenarios(Recursos* R)
{
    Natural i;
    char ruta[MAXLINEA] = {'\0'};

    for (i=0; i<NRO_NIVELES; i++)  // Se cargan los fondos y se leen los mapas de todos los niveles 
    {
        if (i==0)  // El primer nivel no tendrá fondo (solo será una pantalla que cambiará de colores)
        {
            R->fondos[i] = NULL;
        }

        else 
        {
            sprintf(ruta, "assets/images/landscapes/fondo-%hu.png", i+1);
            R->fondos[i] = al_load_bitmap(ruta);

            if (!R->fondos[i])
            {
                printf("Error al cargar el fondo del nivel %hu.\n", i+1);
                return false;
            }

            memset(ruta, '\0', sizeof(ruta));
        }

        R->mapas[i] = leer_mapa(i+1);

        // Si el mapa es NULL, se imprime un mensaje de error
        if (!R->mapas[i].mapa)
        {
            printf("Error al leer el mapa del nivel %d.\n", i);
            return false;
        }
    }

    return true;
}


Procedure liberar_fondos(Imagen fondos[NRO_NIVELES])
{
    Natural i;

    for (i=0; i<NRO_NIVELES; i++)
    {
        if (fondos[i] != NULL)
        {
            al_destroy_bitmap(fondos[i]);
            fondos[i] = NULL;
        }
    }

    return;
}

Procedure liberar_escenarios(Recursos* R)
{
    liberar_mapas(R->mapas);
    liberar_fondos(R->fondos);
}


/**
 * @brief Función que carga los frames (imágenes) correspondientes a un personaje animado según su tipo.
 * Esta función reserva memoria para un arreglo de bitmaps (`Imagen*`) y carga desde disco las imágenes correspondientes al personaje especificado. 
 * Las rutas dependen del tipo de personaje. Si ocurre un error al cargar cualquier frame, la función libera la memoria asignada y devuelve `NULL`.
 * @param tipo Tipo de personaje (`WOOFSON`, `DRAGON`, `EXTRATERRESTRE` o `MONSTRUO`) cuyas imágenes se deben cargar.
 * @return Un puntero al arreglo de `Imagen` (bitmaps de Allegro) si la carga fue exitosa, o `NULL` si hubo un error.
 * @note La memoria retornada debe ser liberada posteriormente con `destruir_frames`.
 * @see destruir_frames
 */
Imagen* cargar_frames(TipoPersonaje tipo)
{
    Natural i;
    Natural cantidad_frames;
    char ruta_base[MAXLINEA] = {'\0'};
    char ruta_completa[MAXLINEA] = {'\0'};
    Imagen* frames = NULL;

    switch (tipo)
    {
        case WOOFSON:
            cantidad_frames = NRO_FRAMES_WOOFSON;
            strcpy(ruta_base, "assets/images/woofson-frames/woofson-");
            break;
            
        case DRAGON:
            cantidad_frames = NRO_FRAMES_DRAGON;
            strcpy(ruta_base, "assets/images/dragon-frames/dragon-");
            break;

        case EXTRATERRESTRE:
            cantidad_frames = NRO_FRAMES_EXTRATERRESTRE;
            strcpy(ruta_base, "assets/images/extraterrestial-frames/extraterrestre-");
            break;

        case MONSTRUO:
            cantidad_frames = NRO_FRAMES_MONSTRUO;
            strcpy(ruta_base, "assets/images/monster-frames/monstruo-");
            break;

        case LEPRECHAUN:
            cantidad_frames = NRO_FRAMES_LEPRECHAUN;
            strcpy(ruta_base, "assets/images/leprechaun-frames/leprechaun-");
            break;

        default:
            cantidad_frames = 0;
            return NULL;
    }

    frames = (Imagen *) calloc(cantidad_frames, sizeof(Imagen));

    if (!frames)
    {
        printf("Error de asignación de memoria al puntero que contendrá los frames del personaje de tipo %c\n", tipo);
        return NULL;
    }

    for (i=0; i<cantidad_frames; i++)
    {
        sprintf(ruta_completa, "%s%hu.png", ruta_base, i+1);

        frames[i] = al_load_bitmap(ruta_completa);

        if (!frames[i])
        {
            printf("Error al cargar el frame %s\n", ruta_completa);
            destruir_frames(frames, cantidad_frames);
            return NULL;
        }

        memset(ruta_completa, '\0', sizeof(ruta_completa));
    }
    
    return frames;
}


bool cargar_todos_los_frames(Imagen* frames[TIPOS_PERSONAJES])
{
    Natural i;

    for (i=0; i<TIPOS_PERSONAJES; i++)
    {
        frames[i] = cargar_frames(tipo_personaje((TipoFrame) i));

        if (!frames[i])
        {
            return false;
        }
    }

    return true;
}


Procedure destruir_frames(Imagen* frames, Natural cantidad_frames)
{
    Natural i;

    for (i=0; i<cantidad_frames; i++) 
    {
        if (frames[i] != NULL) 
        {
            al_destroy_bitmap(frames[i]);
            frames[i] = NULL;
        }
    }

    free(frames);
    frames = NULL;
}


Procedure destruir_todos_los_frames(Imagen* frames[TIPOS_PERSONAJES])
{
    Natural i;
    Natural nro_frames[TIPOS_PERSONAJES] = {NRO_FRAMES_WOOFSON, NRO_FRAMES_DRAGON, NRO_FRAMES_EXTRATERRESTRE, NRO_FRAMES_MONSTRUO, NRO_FRAMES_LEPRECHAUN};

    for (i=0; i<TIPOS_PERSONAJES; i++)
    {
        destruir_frames(frames[(TipoPersonaje) i], nro_frames[i]);
    }
}