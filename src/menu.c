#include "menu.h"

Vector obtener_posicion_mouse()
{
    ALLEGRO_MOUSE_STATE estado_mouse;
    Vector posicion_mouse;

    al_get_mouse_state(&estado_mouse);

    posicion_mouse.x = estado_mouse.x;
    posicion_mouse.y = estado_mouse.y;

    return posicion_mouse;
}


Natural obtener_opcion_en_hover(Menu menu)
{
    Vector posicion_mouse = obtener_posicion_mouse();
    Natural i;

    for (i=0; i<menu.nro_opciones; i++)  /* Se detecta si el mouse esta dentro de alguno de los botones de las opciones del menu */
    {
        if (posicion_mouse.x >= menu.opciones[i].coordenada_inicial.x && posicion_mouse.x <= menu.opciones[i].coordenada_final.x &&
            posicion_mouse.y >= menu.opciones[i].coordenada_inicial.y && posicion_mouse.y <= menu.opciones[i].coordenada_final.y)
        {
            return i;
        }
    }

    return -1;  /* Si no hay ninguna opcion sobrevolada entonces devuelve -1 */
}


/**
 * Función que determina el color de la pantalla según la iteración del juego.
 * @param iteracion Es el número de iteración del juego, que se incrementa cada frame.
 * @param rojo Puntero al valor del componente rojo del color de fondo.
 * @param verde Puntero al valor del componente verde del color de fondo.
 * @param azul Puntero al valor del componente azul del color de fondo.
 */
Procedure determinar_color_pantalla(Natural iteracion)
{
    static Natural rojo, verde, azul;

    if (iteracion % 30 == 0)
    {
        // Cada 30 frames cambia el color de fondo de forma aleatoria
        rojo = rand() % 256;
        verde = rand() % 256;
        azul = rand() % 256;
    }

    al_clear_to_color(al_map_rgb(rojo, verde, azul));
}


bool inicializar_menu_principal(Menu* menu, ALLEGRO_FONT* fuente)
{
    Natural i;
    char* textos_opciones_menu[] = {"JUGAR", "INSTRUCCIONES", "RANKING"};

    menu->nro_opciones = sizeof(textos_opciones_menu) / sizeof(textos_opciones_menu[0]);  /* Se calcula de forma dinámica el numero de opciones del menú */

    menu->opcion_en_hover = -1;  /* Se inicializa en la opcion -1 (que correspondería a USHRT_MAX) para evitar problemas */

    menu->opciones = (Boton *) malloc(menu->nro_opciones * sizeof(Boton));

    if (!menu->opciones)
    {
        printf("Error de asignacion de memoria al inicializar el menu principal.\n");
        return false;
    }

    for (i=0; i<menu->nro_opciones; i++)
    {
        menu->opciones[i].coordenada_inicial.x = ANCHO_VENTANA*0.55;
        menu->opciones[i].coordenada_inicial.y = ALTO_VENTANA*(0.50+0.15*i);

        menu->opciones[i].coordenada_final.x = ANCHO_VENTANA*0.80;
        menu->opciones[i].coordenada_final.y = ALTO_VENTANA*(0.62+0.15*i);

        strcpy(menu->opciones[i].texto, textos_opciones_menu[i]);
    }
        
    menu->fondo = al_load_bitmap("assets/images/menu.jpg");

    if (!menu->fondo)
    {
        printf("Error al cargar el fondo del menu principal.\n");
        finalizar_menu(menu);
        return false;
    }

    menu->fuente = fuente;
    menu->finalizado = false;
    printf("El menu principal ya esta inicializado.\n");

    return true;
}


bool inicializar_menu_niveles(Menu* menu, ALLEGRO_FONT* fuente)
{
    Natural i;
    char* textos_opciones_menu[] = {"1", "2", "3", "4", "5", "Volver atrás"};  /* Se harán los botones de los niveles, más uno para volver para atrás */

    menu->nro_opciones = sizeof(textos_opciones_menu) / sizeof(textos_opciones_menu[0]);  /* Se calcula de forma dinámica el numero de opciones del menú */

    menu->opcion_en_hover = -1;  /* Se inicializa en la opcion -1 (que corresponderia a USHRT_MAX) para evitar problemas */

    menu->opciones = (Boton *) malloc(menu->nro_opciones * sizeof(Boton));
    
    if (!menu->opciones)
    {
        printf("Error de asignacion de memoria al inicializar el menu de niveles.\n");
        return false;
    }

    for (i=0; i<menu->nro_opciones; i++)
    {
        strcpy(menu->opciones[i].texto, textos_opciones_menu[i]);
    }

    for (i=0; i<NRO_NIVELES; i++) /* Asignación de coordenadas para los botones de los niveles */
    {
        menu->opciones[i].coordenada_inicial.x = ANCHO_VENTANA*(0.15 + 0.15*i);
        menu->opciones[i].coordenada_inicial.y = ALTO_VENTANA*0.44;

        menu->opciones[i].coordenada_final.x = ANCHO_VENTANA*(0.25 + 0.15*i);
        menu->opciones[i].coordenada_final.y = ALTO_VENTANA*0.56;
    }

    /* Coordenadas para el botón "Volver atrás" */
    menu->opciones[i].coordenada_inicial.x = ANCHO_VENTANA*0.05;
    menu->opciones[i].coordenada_inicial.y = ALTO_VENTANA*0.80;

    menu->opciones[i].coordenada_final.x = ANCHO_VENTANA*0.20;
    menu->opciones[i].coordenada_final.y = ALTO_VENTANA*0.90; 

    menu->fondo = al_load_bitmap("assets/images/menu_niveles.png");

    if (!menu->fondo)
    {
        printf("Error al cargar el fondo del menu de niveles.\n");
        finalizar_menu(menu);
        return false;
    }

    menu->fuente = fuente;
    menu->finalizado = false;
    printf("El menu de niveles ya esta inicializado.\n");

    return true;
}


Procedure mostrar_menu(Menu menu)
{
    Vector posicion_texto_opcion;
    Natural i;

    menu.opcion_en_hover = obtener_opcion_en_hover(menu);  /* Se obtiene la opcion por la que pasa el cursor (sin seleccionarla aun) */
    
    // Aqui se muestra el fondo del menu
    al_draw_scaled_bitmap(menu.fondo, 0, 0, al_get_bitmap_width(menu.fondo), al_get_bitmap_height(menu.fondo), 0, 0, ANCHO_VENTANA, ALTO_VENTANA, 0);

    if (menu.nro_opciones == NRO_NIVELES+1)
    {
        al_draw_filled_rectangle(ANCHO_VENTANA*0.10, ALTO_VENTANA*0.30, ANCHO_VENTANA*0.90, ALTO_VENTANA*0.70, GRIS);
        al_draw_filled_rectangle(ANCHO_VENTANA*0.30, ALTO_VENTANA*0.22, ANCHO_VENTANA*0.70, ALTO_VENTANA*0.38, AZUL);
        al_draw_text(menu.fuente, BLANCO, ANCHO_VENTANA*0.50, ALTO_VENTANA*0.30, ALLEGRO_ALIGN_CENTRE, "SELECCIONE UN NIVEL:");
    }

    for (i=0; i<menu.nro_opciones; i++)
    {
        al_draw_rectangle(menu.opciones[i].coordenada_inicial.x, menu.opciones[i].coordenada_inicial.y, 
                          menu.opciones[i].coordenada_final.x, menu.opciones[i].coordenada_final.y, 
                          menu.opcion_en_hover == i ? AZUL : NEGRO, 3.0);

        posicion_texto_opcion.x = (menu.opciones[i].coordenada_inicial.x + menu.opciones[i].coordenada_final.x) / 2;
        posicion_texto_opcion.y = (menu.opciones[i].coordenada_inicial.y + menu.opciones[i].coordenada_final.y) / 2;

        al_draw_text(menu.fuente, menu.opcion_en_hover == i ? AZUL : NEGRO, posicion_texto_opcion.x, 
                     posicion_texto_opcion.y, ALLEGRO_ALIGN_CENTRE, menu.opciones[i].texto);
    }

    al_flip_display();  // Se muestra el menu

    return;
}


Procedure redirigir_menu(Menu* menu, ALLEGRO_FONT* fuente, Natural opcion_clickeada, Etapa* etapa_actual)
{
    finalizar_menu(menu);

    if (*etapa_actual == MENU_PRINCIPAL)
    {
        if (opcion_clickeada == 0)
        {
            *etapa_actual = MENU_NIVELES;
            inicializar_menu_niveles(menu, fuente);    
            mostrar_menu(*menu);
        }

        else if (opcion_clickeada == 1)
        {
            al_clear_to_color(ROJO);
            *etapa_actual = INSTRUCCIONES;
            al_flip_display();
        }

        else
        {
            al_clear_to_color(VERDE);
            *etapa_actual = RANKING;
            al_flip_display();
        }
    }

    else if (*etapa_actual == MENU_NIVELES)
    {
        if (opcion_clickeada == 5)
        {
            *etapa_actual = MENU_PRINCIPAL;
            inicializar_menu_principal(menu, fuente);
            mostrar_menu(*menu);
        }

        else
        {
            switch (opcion_clickeada)
            {
                case 0:
                    *etapa_actual = NIVEL1;
                    al_clear_to_color(ROJO);
                    break;

                case 1:
                    *etapa_actual = NIVEL2;
                    al_clear_to_color(VERDE);
                    break;

                case 2:
                    *etapa_actual = NIVEL3;
                    al_clear_to_color(AZUL);
                    break;

                case 3:
                    *etapa_actual = NIVEL4;
                    al_clear_to_color(BLANCO);
                    break;

                case 4:
                    *etapa_actual = NIVEL5;
                    al_clear_to_color(GRIS);
                    break;
            }
            
            al_flip_display();
        }
    }

    return;
}


Procedure finalizar_menu(Menu* menu)
{   
    if (menu->opciones != NULL)
    {
        free(menu->opciones);
        menu->opciones = NULL;
    }

    if (menu->fondo != NULL)
    {
        al_destroy_bitmap(menu->fondo);
        menu->fondo = NULL;
    }
    
    menu->nro_opciones = 0;
    menu->opcion_en_hover = -1;
    menu->finalizado = true;
}


Procedure mostrar_pantalla_datos(Personaje personaje, ALLEGRO_BITMAP* vida)
{
    Natural i;
    Rectangulo rectangulo_datos;

    rectangulo_datos.pos_inicial.x = 0;
    rectangulo_datos.pos_inicial.y = (8./9) * ALTO_VENTANA;
    rectangulo_datos.pos_final.x = ANCHO_VENTANA;
    rectangulo_datos.pos_final.y = ALTO_VENTANA;

    dibujar_rectangulo(rectangulo_datos, GRIS);
    dibujar_vidas(personaje, vida);
    al_draw_rectangle(1./15*ANCHO_VENTANA, 9./10*ALTO_VENTANA, 1./3*ANCHO_VENTANA, 24./25*ALTO_VENTANA, NEGRO, 2);
}


Procedure dibujar_vidas(Personaje personaje, ALLEGRO_BITMAP* vida)
{
    al_draw_bitmap(vida, 100, 820, 0);
}


Procedure dibujar_rectangulo(Rectangulo rectangulo, ALLEGRO_COLOR color)
{
    al_draw_filled_rectangle(rectangulo.pos_inicial.x, rectangulo.pos_inicial.y, rectangulo.pos_final.x, rectangulo.pos_final.y, color);
}
