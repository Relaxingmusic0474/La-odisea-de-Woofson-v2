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


Musica* cargar_musica(TipoAudio tipo_audio)
{
    Musica* musica = (Musica*) malloc(sizeof(Musica));

    if (!musica)
    {
        printf("Error de asignacion de memoria al cargar la musica.\n");
        return NULL;
    }

    // Se inicializa la estructura de la música
    musica->tipo = tipo_audio;
    musica->musica = NULL;
    musica->ID = (ALLEGRO_SAMPLE_ID) {0};

    switch (musica->tipo)
    {
        case MUSICA_MENU:
            musica->musica = al_load_sample("assets/music/Soundtrack-menu.ogg");
            break;

        case MUSICA_NIVEL_1:
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
        printf("Error al cargar la musica.\n");
        free(musica);
        musica = NULL;
    }

    return musica;
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
    Mapa mapa = {NULL, 0, 0};
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
                al_draw_filled_rectangle(j*ANCHO_VENTANA/mapa.nro_columnas, i*810/mapa.nro_filas, 
                                        (j+1)*ANCHO_VENTANA/mapa.nro_columnas, (i+1)*810/mapa.nro_filas, CAFE);
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