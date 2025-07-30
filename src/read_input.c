#include "read_input.h"

void leer_entrada_datos(FILE* archivo, Datos data[MAX_DATOS])
{
    Natural i;
    int tam = 1;
    char ch = '\0';

    printf("%ld\n", ftell(archivo));

    for (i=0; i<MAX_DATOS; i++)
    {
        memset(data[i].nombre, '\0', sizeof(data[i].nombre));
    }

    for (i=0; i<MAX_DATOS; i++)
    {
        while (tam < LARGO && (ch = fgetc(archivo)) != '\t' && ch != EOF) 
        {
            data[i].nombre[tam++] = ch;
        }

        data->nombre[tam-1] = '\0';
        fscanf(archivo, "%hu", &data->puntaje);
        while ((ch = fgetc(archivo)) != '\n');
    }

    return;
}
