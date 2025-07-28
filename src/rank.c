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


Procedure ingresar_nombre(char* nombre, size_t largo_max, ALLEGRO_FONT* fuente, ALLEGRO_EVENT_QUEUE* cola_eventos)
{
    ALLEGRO_EVENT evento;
    Tecla tecla;
    size_t len = 0;
    
    nombre[0] = '\0';

    LOOP
    {
        al_wait_for_event(cola_eventos, &evento);

        if (evento.type == ALLEGRO_EVENT_KEY_CHAR)
        {
            tecla = evento.keyboard.unichar;

            if (tecla == '\r' || tecla == '\n')  // Enter
            {
                break;
            }

            if (tecla == '\b' && len > 0)  // Borrado (backspace)
            {
                len--;
                nombre[len] = '\0';
            }

            else if (isprint(tecla) && len < largo_max - 1)
            {
                nombre[len++] = (char) tecla;
                nombre[len] = '\0';
            }
        }

        if (evento.type == ALLEGRO_EVENT_TIMER || evento.type == ALLEGRO_EVENT_KEY_CHAR)
        {
            al_clear_to_color(NEGRO);
            al_draw_textf(fuente, BLANCO, ANCHO_VENTANA/2, ALTO_VENTANA/2, ALLEGRO_ALIGN_CENTER, "Ingrese su nombre: %s", nombre);
            al_flip_display();
        }
    }
}