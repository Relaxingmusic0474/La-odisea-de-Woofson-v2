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


Procedure redirigir_menu(Recursos* recursos, Menu* menu, ALLEGRO_FONT* fuente, Natural opcion_clickeada, Etapa* etapa_actual)
{
    finalizar_menu(&recursos->menu_actual);

    if (*etapa_actual == MENU_PRINCIPAL)
    {
        if (opcion_clickeada == 0)  
        {
            *etapa_actual = MENU_NIVELES;
            inicializar_menu_niveles(&recursos->menu_actual, fuente);    
            mostrar_menu(recursos->menu_actual);
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
            inicializar_menu_principal(&recursos->menu_actual, fuente);
            mostrar_menu(recursos->menu_actual);
        }

        else
        {
            switch (opcion_clickeada)
            {
                case 0:
                    *etapa_actual = NIVEL1;
                    cambiar_musica(recursos, recursos->musicas[1]);
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


Procedure dibujar_rectangulo(Rectangulo rectangulo, ALLEGRO_COLOR color)
{
    al_draw_filled_rectangle(rectangulo.pos_inicial.x, rectangulo.pos_inicial.y, rectangulo.pos_final.x, rectangulo.pos_final.y, color);
}


Procedure dibujar_imagen_en_rectangulo(Imagen imagen, Rectangulo rectangulo, float porcentaje_x, float porcentaje_y)
{
    Vector posicion_dibujo;
    int alto_imagen = al_get_bitmap_height(imagen);
    posicion_dibujo.x = rectangulo.pos_inicial.x + (float) porcentaje_x / 100 * (rectangulo.pos_final.x - rectangulo.pos_inicial.x);
    posicion_dibujo.y = rectangulo.pos_inicial.y + (float) porcentaje_y / 100 * (rectangulo.pos_final.y - rectangulo.pos_inicial.y);
    al_draw_bitmap(imagen, posicion_dibujo.x, posicion_dibujo.y - (float) alto_imagen / 2, ALLEGRO_ALIGN_CENTRE);
}


Procedure dibujar_texto_en_rectangulo(char* texto, Rectangulo rectangulo, float porcentaje_x, float porcentaje_y, ALLEGRO_FONT* fuente)
{
    Vector posicion_texto;
    int alto_texto = al_get_font_line_height(fuente);
    posicion_texto.x = rectangulo.pos_inicial.x + (float) porcentaje_x / 100 * (rectangulo.pos_final.x - rectangulo.pos_inicial.x);
    posicion_texto.y = rectangulo.pos_inicial.y + (float) porcentaje_y / 100 * (rectangulo.pos_final.y - rectangulo.pos_inicial.y);
    al_draw_text(fuente, NEGRO, posicion_texto.x, posicion_texto.y - (float) alto_texto / 2, ALLEGRO_ALIGN_CENTRE, texto);
}


Procedure dibujar_rectangulo_en_rectangulo(Rectangulo rect_destino, float alto, float ancho, float porcentaje_x, float porcentaje_y, bool relleno, ALLEGRO_COLOR color)
{
    Vector posicion_inicial, posicion_final;

    posicion_inicial.x = rect_destino.pos_inicial.x + porcentaje_x / 100 * (rect_destino.pos_final.x - rect_destino.pos_inicial.x) - ancho / 2;
    posicion_inicial.y = rect_destino.pos_inicial.y + porcentaje_y / 100 * (rect_destino.pos_final.y - rect_destino.pos_inicial.y) - alto / 2;

    posicion_final.x = rect_destino.pos_inicial.x + porcentaje_x / 100 * (rect_destino.pos_final.x - rect_destino.pos_inicial.x) + ancho / 2;
    posicion_final.y = rect_destino.pos_inicial.y + porcentaje_y / 100 * (rect_destino.pos_final.y - rect_destino.pos_inicial.y) + alto / 2;
    
    if (relleno)
    {
        al_draw_filled_rectangle(posicion_inicial.x, posicion_inicial.y, posicion_final.x, posicion_final.y, color);
    }

    else
    {
        al_draw_rectangle(posicion_inicial.x, posicion_inicial.y, posicion_final.x, posicion_final.y, color, 2.0);
    }
}


Procedure mostrar_pantalla_datos(Personaje personaje, ALLEGRO_BITMAP* vida, ALLEGRO_FONT* fuente, ALLEGRO_FONT* fuente_sec, Etapa etapa_actual)
{
    Natural i;
    Natural nivel, alto_linea;
    Rectangulo rectangulo_datos;
    char texto_nro_nivel[6] = {'\0'};
    char texto_nro_vidas[5] = {'\0'};

    rectangulo_datos.pos_inicial.x = 0;
    rectangulo_datos.pos_inicial.y = (8./9) * ALTO_VENTANA;
    rectangulo_datos.pos_final.x = ANCHO_VENTANA;
    rectangulo_datos.pos_final.y = ALTO_VENTANA;

    nivel = etapa_actual + 1;
    alto_linea = rectangulo_datos.pos_final.y - rectangulo_datos.pos_inicial.y;

    dibujar_rectangulo(rectangulo_datos, GRIS);
    sprintf(texto_nro_nivel, "Lvl %hu", nivel);
    dibujar_texto_en_rectangulo(texto_nro_nivel, rectangulo_datos, 5.0, 50.0, fuente);
    dibujar_rectangulo_en_rectangulo(rectangulo_datos, alto_linea, 0, 10.0, 50.0, false, NEGRO);  // Esto es equivalente a dibujar una línea (una de las dimensiones es 0)
    dibujar_imagen_en_rectangulo(vida, rectangulo_datos, 12.5, 35.0);  // Se dibuja el corazón en un 5% en x y un 50% en y del rectángulo
    sprintf(texto_nro_vidas, "× %hu", personaje.nro_vidas);
    dibujar_texto_en_rectangulo(texto_nro_vidas, rectangulo_datos, 18.0, 35.0, fuente);
    dibujar_texto_en_rectangulo("VIDAS", rectangulo_datos, 16.3, 75.0, fuente_sec);
    dibujar_rectangulo_en_rectangulo(rectangulo_datos, alto_linea, 0, 22.5, 50.0, false, NEGRO);
    dibujar_rectangulo_en_rectangulo(rectangulo_datos, 50, 800, 50.0, 35.0, false, NEGRO);
    dibujar_texto_en_rectangulo("BARRA DE VIDA", rectangulo_datos, 50.0, 75.0, fuente_sec);
    // al_draw_rectangle(1./15*ANCHO_VENTANA, 9./10*ALTO_VENTANA, 1./3*ANCHO_VENTANA, 24./25*ALTO_VENTANA, NEGRO, 2);
}