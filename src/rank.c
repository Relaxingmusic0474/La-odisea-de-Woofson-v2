#include "rank.h"

bool leer_ranking(Ranking* ranking, Natural nro_nivel)
{
    Natural i;
    FILE* archivo_ranking = NULL;
    char nombre_archivo[LARGO] = {'\0'};

    sprintf(nombre_archivo, "assets/rankings/ranking-%hu.txt", nro_nivel);

    archivo_ranking = fopen(nombre_archivo, "r");

    if (!archivo_ranking)
    {
        printf("Error al abrir el archivo del ranking del nivel %hu\n", nro_nivel);
        return false;
    }

    i = 0;

    while (i < MAX_DATOS && fscanf(archivo_ranking, "%s %hu", ranking->datos[i].nombre, &ranking->datos[i].puntaje) == 2)
    {
        ranking->datos[i].nro_nivel = nro_nivel;
        i++;
    }

    fclose(archivo_ranking);
    return true;
}