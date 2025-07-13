#include "traps.h"

Procedure inicializar_rayo(Rayo* rayo)
{
    rayo->origen.x = 0;
    rayo->origen.y = 0;
    rayo->objetivo.x = 0;
    rayo->objetivo.y = 0;
    rayo->posicion.x = 0;
    rayo->posicion.y = 0;
    rayo->velocidad.x = 0;
    rayo->velocidad.y = 0;
    rayo->danho = DANHO_RAYO;  // Asigna el daño del rayo
    rayo->activo = false;  // Inicialmente el rayo no está activo
}

Procedure activar_rayo(Rayo* rayo)
{
    rayo->activo = true;  // Activa el rayo
    rayo->posicion = rayo->origen;  // Resetea la posición del rayo al origen
}

Procedure desactivar_rayo(Rayo* rayo)
{
    rayo->activo = false;  // Desactiva el rayo
}

Natural detectar_rayos(Mapa mapa, Rayo rayos[], Natural max_rayos)
{
    Natural i, j, k, t, cantidad;  // Contadores y acumuladores
    bool es_valido;  // Variable para verificar si el rayo es válido

    cantidad = 0;  // Inicializa el contador de rayos detectados

    // Recorre el mapa para detectar pares de trampas (un rayo se forma entre dos bloques trampa ubicados en la misma fila o columna)
    for (i=0; i<mapa.nro_filas; i++)
    {
        for (j=0; j<mapa.nro_columnas; j++)
        {
            if (mapa.mapa[i][j] == TRAMPA)
            {
                // Buscar en dirección horizontal
                for (k=j+1; k<mapa.nro_columnas; k++)
                {
                    if (mapa.mapa[i][k] == TRAMPA)
                    {
                        // Se verifica que no haya bloques entre las dos trampas (porque el rayo no puede atravesar bloques)
                        es_valido = true;

                        for (t=j+1; t<k; t++)
                        {
                            if (mapa.mapa[i][t] != NADA)
                            {
                                es_valido = false;
                                break;
                            }
                        }

                        if (es_valido && cantidad < max_rayos)
                        {
                            rayos[cantidad].origen.x = (j + 0.5f) * mapa.ancho_bloque;
                            rayos[cantidad].origen.y = (i + 0.5f) * mapa.alto_bloque;
                            rayos[cantidad].objetivo.x = (k + 0.5f) * mapa.ancho_bloque;
                            rayos[cantidad].objetivo.y = (i + 0.5f) * mapa.alto_bloque;
                            cantidad++;
                        }

                        break;  // Solo detectamos el primer par
                    }
                }

                // Buscar en dirección vertical
                for (k=i+1; k<mapa.nro_filas; k++)
                {
                    if (mapa.mapa[k][j] == TRAMPA)
                    {
                        es_valido = true;

                        for (t=i+1; t<k; t++)
                        {
                            if (mapa.mapa[t][j] != 0)
                            {
                                es_valido = false;
                                break;
                            }
                        }

                        if (es_valido && cantidad < max_rayos)
                        {
                            rayos[cantidad].origen.x = (j + 0.5f) * mapa.ancho_bloque;
                            rayos[cantidad].origen.y = (i + 0.5f) * mapa.alto_bloque;
                            rayos[cantidad].objetivo.x = (j + 0.5f) * mapa.ancho_bloque;
                            rayos[cantidad].objetivo.y = (k + 0.5f) * mapa.alto_bloque;
                            cantidad++;
                        }

                        break;
                    }
                }
            }
        }
    }

    return cantidad;
}