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
        printf("Error al abrir en modo de lectura el archivo del ranking del nivel %hu\n", nro_nivel);
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


bool modificar_ranking(Ranking* ranking, Natural nro_nivel)
{
    Natural i;
    FILE* archivo_ranking = NULL;
    char nombre_archivo[LARGO] = {'\0'};

    sprintf(nombre_archivo, "assets/rankings/ranking-%hu.txt", nro_nivel);

    archivo_ranking = fopen(nombre_archivo, "w");

    if (!archivo_ranking)
    {
        printf("Error al abrir en modo de escritura el archivo del ranking del nivel %hu\n", nro_nivel);
        return false;
    }

    for (i=0; i<MAX_DATOS; i++)
    {
        fprintf(archivo_ranking, "%s %hu\n", ranking->datos[i].nombre, ranking->datos[i].puntaje);
    }

    fclose(archivo_ranking);
    return true;
}


Procedure intercambiar_datos(Datos* data_1, Datos* data_2)
{
    Datos aux;
    aux = *data_1;
    *data_1 = *data_2;
    *data_2 = aux;
}


Procedure insertar_en_ranking(Ranking* ranking, char* nombre, Natural nivel)
{
    Natural i, j;
    Datos data = {0};

    if (puntuacion <= ranking->datos[MAX_DATOS-1].puntaje)  // Si no sobrepasa a la puntuación del último top 10, no entra en el ranking
    {
        return;
    }

    strcpy(data.nombre, nombre);
    data.puntaje = puntuacion;
    data.nro_nivel = nivel;

    memset(ranking->datos[MAX_DATOS-1].nombre, '\0', sizeof(ranking->datos[MAX_DATOS-1].nombre));
    ranking->datos[MAX_DATOS-1] = data;

    for (i=MAX_DATOS-1; i>0; i--)
    {
        if (ranking->datos[i].puntaje < ranking->datos[i-1].puntaje)  // Puntuación es una variable global
        {
            intercambiar_datos(&ranking->datos[i], &ranking->datos[i-1]);
        }
        
        else
        {
            break;
        }
    }
}