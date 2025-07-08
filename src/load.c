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
            musica->musica = al_load_sample("assets/music/Soundtrack-A.ogg");
            break;

        case MUSICA_NIVEL_2:
            break;

        case MUSICA_NIVEL_3:
            break;

        case MUSICA_NIVEL_4:
            break;

        case MUSICA_NIVEL_5:
            break;

        default:
            printf("Tipo de audio no valido.\n");
            return NULL;
    }

    if (!musica->musica)
    {
        printf("Error al cargar el sample %s.\n", tipo == MUSICA_MENU ? "Soundtrack-menu.ogg" : "Soundtrack-A.ogg");
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

    if (!al_attach_sample_instance_to_mixer(musica->instancia, mixer))
    {
        printf("Error al adjuntar al mixer la instancia de la música %s.\n", tipo == MUSICA_MENU ? "Soundtrack-menu.ogg" : "Soundtrack-A.ogg");
        al_destroy_sample_instance(musica->instancia);
        musica->instancia = NULL;
        al_destroy_sample(musica->musica);
        musica->musica = NULL;
        return NULL;
    }

    return musica;
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
Procedure dibujar_mapa(Mapa mapa)
{
    Natural i=0, j=0;
    
    for (i=0; i<mapa.nro_filas; i++)
    {
        for (j=0; j<mapa.nro_columnas; j++)
        {
            if (mapa.mapa[i][j] == 1)
            {
                al_draw_filled_rectangle(j*ANCHO_VENTANA/mapa.nro_columnas, i*ALTURA_PISO/mapa.nro_filas, 
                                        (j+1)*ANCHO_VENTANA/mapa.nro_columnas, (i+1)*ALTURA_PISO/mapa.nro_filas, CAFE);
            }
        }
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