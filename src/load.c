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