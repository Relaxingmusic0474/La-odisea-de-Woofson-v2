#include "allegro.h"
#include "debug.h"

Procedure mostrar_mapa(Mapa mapa)
{
    Natural i, j;

    printf("\n");

    for (i=0; i<mapa.nro_filas; i++)
    {
        for (j=0; j<mapa.nro_columnas; j++)
        {
            printf("%hu ", mapa.mapa[i][j]);  /* Imprime el valor del bloque en la posición (i, j) */
        }

        printf("\n");  /* Nueva línea al final de cada fila */
    }

    printf("\n- Alto del bloque: %.1f\n", mapa.alto_bloque);
    printf("- Ancho del bloque: %.1f\n\n", mapa.ancho_bloque);
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
    caida_libre_activada(personaje);
}


Procedure mostrar_bloque_actual_personaje(Personaje personaje, Mapa mapa)
{
    // Para colisión inferior
    float x_central = personaje.posicion.x + personaje.ancho / 2;
    float y_inf = personaje.posicion.y + personaje.alto;

    printf("Posición: (y = %.2f, x = %.2f)\n", y_inf, x_central);
    printf("Bloque actual: (fila: %d, col: %d)\n", (int) y_inf / (int) mapa.alto_bloque, (int) x_central / (int) mapa.ancho_bloque);
}



Procedure caida_libre_activada(Personaje p)
{
    p.velocidad.y > 0 ? printf("Estoy en caida libre\n") : printf("No estoy en caida libre\n");
}

