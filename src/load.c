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