#include "menu.h"

Posicion obtener_posicion_mouse()
{
    ALLEGRO_MOUSE_STATE estado_mouse;
    Posicion posicion_mouse;

    al_get_mouse_state(&estado_mouse);

    posicion_mouse.x = estado_mouse.x;
    posicion_mouse.y = estado_mouse.y;

    return posicion_mouse;
};


Natural obtener_opcion_en_hover(Menu menu)
{
    Posicion posicion_mouse = obtener_posicion_mouse();
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
};


bool inicializar_menu_principal(Menu* menu)
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
        menu->opciones[i].coordenada_inicial.x = SCREEN_WIDTH*0.55;
        menu->opciones[i].coordenada_inicial.y = SCREEN_HEIGHT*(0.50+0.15*i);

        menu->opciones[i].coordenada_final.x = SCREEN_WIDTH*0.80;
        menu->opciones[i].coordenada_final.y = SCREEN_HEIGHT*(0.62+0.15*i);

        strcpy(menu->opciones[i].texto, textos_opciones_menu[i]);
    }
        
    menu->fondo = al_load_bitmap("menu.jpg");

    if (!menu->fondo)
    {
        printf("Error al cargar el fondo del menu principal.\n");
        finalizar_menu(menu);
        return false;
    }

    menu->fuente = cargar_fuente(TIMES_NEW_ROMAN, GRANDE);

    if (!menu->fuente)
    {
        printf("Error al cargar la fuente del menu principal.\n");
        finalizar_menu(menu);
        return false;
    }

    menu->inicializado = true;

    printf("El menu principal ya esta inicializado.\n");

    return true;
};


bool inicializar_menu_niveles(Menu* menu)
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
        menu->opciones[i].coordenada_inicial.x = SCREEN_WIDTH*(0.15 + 0.15*i);
        menu->opciones[i].coordenada_inicial.y = SCREEN_HEIGHT*0.44;

        menu->opciones[i].coordenada_final.x = SCREEN_WIDTH*(0.25 + 0.15*i);
        menu->opciones[i].coordenada_final.y = SCREEN_HEIGHT*0.56;
    }

    /* Coordenadas para el botón "Volver atrás" */
    menu->opciones[i].coordenada_inicial.x = SCREEN_WIDTH*0.05;
    menu->opciones[i].coordenada_inicial.y = SCREEN_HEIGHT*0.80;

    menu->opciones[i].coordenada_final.x = SCREEN_WIDTH*0.20;
    menu->opciones[i].coordenada_final.y = SCREEN_HEIGHT*0.90; 

    menu->fondo = al_load_bitmap("menu_niveles.png");

    if (!menu->fondo)
    {
        printf("Error al cargar el fondo del menu de niveles.\n");
        finalizar_menu(menu);
        return false;
    }

    menu->fuente = cargar_fuente(COMFORTAA_LIGHT, GRANDE);

    if (!menu->fuente)
    {
        printf("Error al cargar la fuente del menu de niveles.\n");
        finalizar_menu(menu);
        return false;
    }

    menu->inicializado = true;

    printf("El menu de niveles ya esta inicializado.\n");

    return true;
};


Procedure mostrar_menu(Menu menu)
{
    Posicion posicion_texto_opcion;
    Natural i;

    menu.opcion_en_hover = obtener_opcion_en_hover(menu);  /* Se obtiene la opcion por la que pasa el cursor (sin seleccionarla aun) */
    
    // Aqui se muestra el fondo del menu
    al_draw_scaled_bitmap(menu.fondo, 0, 0, al_get_bitmap_width(menu.fondo), al_get_bitmap_height(menu.fondo), 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

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


Procedure redirigir_menu(Menu* menu, Natural opcion_clickeada, Etapa* etapa_actual)
{
    finalizar_menu(menu);

    if (*etapa_actual == MENU_PRINCIPAL)
    {
        if (opcion_clickeada == 0)
        {
            *etapa_actual = MENU_NIVELES;
            inicializar_menu_niveles(menu);    
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
            inicializar_menu_principal(menu);
            mostrar_menu(*menu);
        }
    }


    return;
}

Procedure finalizar_menu(Menu* menu)
{
    if (menu->fuente != NULL)
    {
        al_destroy_font(menu->fuente);
        menu->fuente = NULL;
    }
    
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
    menu->inicializado = false;

    return;   
}