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
        printf("Error al cargar el archivo del mapa\n");
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
        return NULL;
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
Natural** leer_mapa(Natural nro_nivel, Natural* nro_filas, Natural* nro_columnas)
{
    Natural i=0, j=0;
    char ch = '\0';
    Natural** mapa = NULL;
    FILE* archivo_mapa = NULL;

    *nro_filas = 0, *nro_columnas = 0;  /* Por seguridad se inicializa en 0 */

    *nro_filas = obtener_nro_filas(nro_nivel);

    if (*nro_filas == 0)
    {
        return NULL;
    }

    *nro_columnas = obtener_nro_columnas(nro_nivel);

    if (*nro_columnas == 0)
    {
        return NULL;
    }

    mapa = (Natural **) malloc(*nro_filas * sizeof(Natural*));

    if (!mapa)
    {
        printf("Error de asignación de memoria");
        return NULL;
    }

    for (i=0; i<*nro_filas; i++)
    {
        mapa[i] = (Natural *) malloc(*nro_columnas * sizeof(Natural));

        if (!mapa[i])
        {
            printf("Error de asignación de memoria");

            for (j=0; j<i; j++)
            {
                free(mapa[j]);
            }

            free(mapa);

            return NULL;
        }
    }

    archivo_mapa = cargar_mapa(nro_nivel);

    if (!archivo_mapa)
    {
        return NULL;
    }

    i=0, j=0;

    // Aqui el codigo para leer el archivo y asignar/reasignar dinamicamente el tamaño de la matriz dependiendo los datos que tenga el archivo
    while (fscanf(archivo_mapa, "%hu ", &mapa[i][j]) == 1)
    {
        j = (j+1) % *nro_columnas;
        
        if (j == 0)
        {
            i++;
        }
    }

    fclose(archivo_mapa);

    return mapa;
}


Procedure dibujar_mapa(Natural* mapa[], Natural nro_filas, Natural nro_columnas)
{
    Natural i=0, j=0;
    
    for (i=0; i<nro_filas; i++)
    {
        for (j=0; j<nro_columnas; j++)
        {
            if (mapa[i][j] == 1)
            {
                al_draw_filled_rectangle(j*ANCHO_VENTANA/nro_columnas, i*810/nro_filas, (j+1)*ANCHO_VENTANA/nro_columnas, (i+1)*810/nro_filas, AZUL);
            }
        }
    }

    return;
}


Procedure liberar_mapa(Natural* mapa[], Natural nro_filas)
{
    Natural i=0;

    for (i=0; i<nro_filas; i++)
    {
        free(mapa[i]);
        mapa[i] = NULL;
    }

    free(mapa);
    mapa = NULL;

    return;
}