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
    printf("Direccion: %s\n", personaje.direccion == 1 ? "DERECHA" : (personaje.direccion == -1 ? "IZQUIERDA" : "INDETERMINADA"));
    printf("Bandera de dibujo: %d\n", personaje.bandera_dibujo);
    printf("Salto: %s\n", personaje.salto.en_salto ? "Sí" : "No");
    printf("En plataforma: %s\n", personaje.en_plataforma ? "Sí" : "No");
    printf("Altura inicial del salto: %.2f\n", personaje.salto.altura_inicial);
    printf("Tiempo en salto: %.2f\n", personaje.salto.tiempo_en_salto);
    printf("Impulso del salto: %.2f\n", personaje.salto.impulso);
    printf("Vidas: %hu\n", personaje.nro_vidas);
    printf("Subvida actual: %hu\n", personaje.subvida_actual);
    printf("Frames para el proximo disparo: %hu\n", personaje.frames_para_prox_disparo);
    printf("Tiempo de daño: %.4f s\n", personaje.tiempo_danho);
    printf("Muerto: %s\n", personaje.muerto ? "Sí" : "No");
    printf("Ancho: %.2f\n", personaje.ancho);
    caida_libre_activada(personaje);
}


Procedure mostrar_bloque_actual_personaje(Personaje personaje, Mapa mapa)
{
    // Para colisión inferior
    float x_central = personaje.posicion.x + personaje.ancho / 2;
    float y_inf = personaje.posicion.y + personaje.alto;

    printf("Posición: (y = %.2f, x = %.2f)\n", y_inf, x_central);
    printf("Bloque actual: (fila: %d, col: %d)\n\n", (int) y_inf / (int) mapa.alto_bloque, (int) x_central / (int) mapa.ancho_bloque);
}


Procedure caida_libre_activada(Personaje p)
{
    p.velocidad.y > 0 ? printf("Estoy en caida libre\n") : printf("No estoy en caida libre\n");
}

Procedure debug_rayo_personaje(Rayo* rayo, Personaje* personaje)
{
    printf("Rayo activo: %d\n", rayo->activo);
    printf("Rayo origen: (%.2f, %.2f)\n", rayo->origen.x, rayo->origen.y);
    printf("Rayo posicion actual: (%.2f, %.2f)\n", rayo->posicion.x, rayo->posicion.y);
    printf("Personaje rect: (%.2f, %.2f) a (%.2f, %.2f)\n", personaje->posicion.x, personaje->posicion.y, personaje->posicion.x + personaje->ancho, personaje->posicion.y + personaje->alto);
}

Procedure imprimir_datos_rayos(Rayo rayos[MAX_RAYOS], Natural cantidad)
{
    Natural i;

    if (cantidad > MAX_RAYOS)
    {
        return;
    }

    for (i=0; i<cantidad; i++)
    {
        printf("Origen: (%.1f, %.1f)\n", rayos[i].origen.x, rayos[i].origen.y);
        printf("Destino: (%.1f, %.1f)\n", rayos[i].objetivo.x, rayos[i].objetivo.y);
    }
}

Procedure imprimir_posicion_balas(Bala balas[MAX_BALAS])
{
    Natural i;

    for (i=0; i<MAX_BALAS; i++)
    {
        printf("Bala %hu en posición (%.2f, %.2f)\n", i+1, balas[i].posicion.x, balas[i].posicion.y);
    }

    printf("\n");
}

Procedure mostrar_estados_actuales(Etapa etapa_actual, Natural nivel_actual)
{
    char* etapas[] = {"MENU PRINCIPAL", "MENU NIVELES", "INSTRUCCIONES", "RANKING", "NIVEL 1", "NIVEL 2", "NIVEL 3", "NIVEL 4", "NIVEL 5", "DERROTA", "VICTORIA", "MENU RANKING"};

    printf("Etapa actual: %s\n", etapas[etapa_actual+4]);
    printf("Nivel actual: %hu\n\n", nivel_actual);
}

Procedure imprimir_posicion_charcos(Charco charcos[MAX_CHARCOS])
{
    Natural i;

    if ((rand() % 50) + 470 == 474)
    {
        for (i=0; i<MAX_CHARCOS; i++)
        {
            if (charcos[i].activo)
            {
                printf("- Charco %hu en la posicion (%.2f, %2f)\n", i+1, charcos[i].posicion.x, charcos[i].posicion.y);
            }

            else
            {
                printf("- Charco %hu desactivado\n", i+1);
            }
        }
    }
}