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

    for (i=0; i<2; i++)  /* Se detecta si el mouse esta dentro de alguno de los botones de las opciones del menu */
    {
        if (posicion_mouse.x >= menu.opciones[i].coordenada_inicial.x && posicion_mouse.x <= menu.opciones[i].coordenada_final.x &&
            posicion_mouse.y >= menu.opciones[i].coordenada_inicial.y && posicion_mouse.y <= menu.opciones[i].coordenada_final.y)
        {
            return i;
        }
    }

    return -1;  /* Si no hay ninguna opcion sobrevolada entonces devuelve -1 */
};


bool inicializar_menu(Menu* menu)
{
    menu->fondo = al_load_bitmap("menu.jpg");  // Se carga el fondo del menu

    if (!menu->fondo)
    {
        printf("Error al cargar el fondo del menu.\n");
        return false;
    }

    menu->fuente = cargar_fuente(TIMES_NEW_ROMAN, GRANDE);

    if (!menu->fuente)
    {
        printf("Error al cargar la fuente del menu.\n");
        return false;
    }

    menu->opciones[0].coordenada_inicial.x = SCREEN_WIDTH*0.55;
    menu->opciones[0].coordenada_inicial.y = SCREEN_HEIGHT*0.55;

    menu->opciones[0].coordenada_final.x = SCREEN_WIDTH*0.80;
    menu->opciones[0].coordenada_final.y = SCREEN_HEIGHT*0.70;

    menu->opciones[1].coordenada_inicial.x = SCREEN_WIDTH*0.55;
    menu->opciones[1].coordenada_inicial.y = SCREEN_HEIGHT*0.75;

    menu->opciones[1].coordenada_final.x = SCREEN_WIDTH*0.80;
    menu->opciones[1].coordenada_final.y = SCREEN_HEIGHT*0.90;

    menu->opcion_en_hover = NRO_OPCIONES_MENU;  /* Se inicializa en la opcion N para evitar problemas, ya que las opciones van desde la 0 a la N-1 */

    return true;
};


Procedure mostrar_menu(Menu menu)
{
    Posicion posicion_texto_opcion;
    Natural i;

    menu.opcion_en_hover = obtener_opcion_en_hover(menu);  /* Se obtiene la opcion por la que pasa el cursor (sin seleccionarla aun) */
    
    // Aqui se muestra el fondo del menu
    al_draw_scaled_bitmap(menu.fondo, 0, 0, al_get_bitmap_width(menu.fondo), al_get_bitmap_height(menu.fondo), 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    for (i=0; i<2; i++)
    {
        al_draw_rectangle(menu.opciones[i].coordenada_inicial.x, menu.opciones[i].coordenada_inicial.y, 
                          menu.opciones[i].coordenada_final.x, menu.opciones[i].coordenada_final.y, 
                          menu.opcion_en_hover == i ? AZUL : NEGRO, 3.0);

        posicion_texto_opcion.x = (menu.opciones[i].coordenada_inicial.x + menu.opciones[i].coordenada_final.x) / 2;
        posicion_texto_opcion.y = (menu.opciones[i].coordenada_inicial.y + menu.opciones[i].coordenada_final.y) / 2;

        al_draw_text(menu.fuente, menu.opcion_en_hover == i ? AZUL : NEGRO, posicion_texto_opcion.x, 
                     posicion_texto_opcion.y, ALLEGRO_ALIGN_CENTRE, i==0 ? "JUGAR" : "INSTRUCCIONES");
    }

    al_flip_display();  // Se muestra el menu

    return;
}


Procedure redirigir_menu(Menu* menu, Natural opcion_clickeada, Etapa* etapa_actual)
{
    if (menu->fondo != NULL)
    {
        al_destroy_bitmap(menu->fondo);
        menu->fondo = NULL;
    }

    if (opcion_clickeada == 0)
    {
        al_clear_to_color(ROJO);
        *etapa_actual = MENU_NIVELES;
    }

    else
    {
        al_clear_to_color(VERDE);
        *etapa_actual = INSTRUCCIONES;
    }

    al_flip_display();

    return;
}