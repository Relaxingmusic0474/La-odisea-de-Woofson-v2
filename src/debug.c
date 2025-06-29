#include "allegro.h"
#include "debug.h"

Procedure mostrar_mapa(Mapa mapa)
{
    Natural i, j;

    for (i=0; i<mapa.nro_filas; i++)
    {
        for (j=0; j<mapa.nro_columnas; j++)
        {
            printf("%hu ", mapa.mapa[i][j]);  /* Imprime el valor del bloque en la posición (i, j) */
        }

        printf("\n");  /* Nueva línea al final de cada fila */
    }
}

Procedure mostrar_datos_personaje(Personaje personaje)
{
    printf("Posición: (%.2f, %.2f)\n", personaje.posicion.x, personaje.posicion.y);
    printf("Velocidad: (%.2f, %.2f)\n", personaje.velocidad.x, personaje.velocidad.y);
    printf("Salto: %s\n", personaje.salto.en_salto ? "Sí" : "No");
    printf("En plataforma: %s\n", personaje.en_plataforma ? "Sí" : "No");
    printf("Altura inicial del salto: %.2f\n", personaje.salto.altura_inicial);
    printf("Tiempo en salto: %.2f\n", personaje.salto.tiempo_en_salto);
    printf("Impulso del salto: %.2f\n", personaje.salto.impulso);
}




