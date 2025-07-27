#include "rank.h"

bool leer_ranking(Ranking* ranking, Natural nro_nivel)
{
    FILE* archivo_ranking = NULL;
    char nombre_archivo[LARGO] = {'\0'};

    sprintf(nombre_archivo, "assets/rankings/ranking-%hu.txt", nro_nivel);

    archivo_ranking = fopen(nombre_archivo, "r");

    if (!archivo_ranking)
    {
        printf("Error al abrir el archivo del ranking del nivel %hu\n", nro_nivel);
        return false;
    }

    
    

}