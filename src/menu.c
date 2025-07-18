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
 */
Procedure determinar_color_pantalla(Natural iteracion)
{
    ALLEGRO_COLOR lista_colores[NRO_COLORES] = {AZUL, ROSADO, MORADO, VERDE_OSCURO, ROJO};
    static Natural i = 0;  // Estática para que ignore el valor inicial
    ALLEGRO_COLOR color = lista_colores[i];

    if (iteracion % 30 == 0)
    {
        color = lista_colores[i];
        i = (i + 1) % NRO_COLORES;
        
    }

    al_clear_to_color(color);
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


Procedure redirigir_menu(Recursos* recursos, ALLEGRO_FONT* fuente, Natural opcion_clickeada, Etapa* etapa_actual, Natural* nivel_actual)
{
    finalizar_menu(&recursos->menu_actual);

    if (*etapa_actual == MENU_PRINCIPAL)
    {
        *nivel_actual = 0;

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
            *nivel_actual = 0;
            *etapa_actual = MENU_PRINCIPAL;
            inicializar_menu_principal(&recursos->menu_actual, fuente);
            mostrar_menu(recursos->menu_actual);
        }

        else
        {
            switch (opcion_clickeada)
            {
                case 0:
                    *nivel_actual = 1;
                    *etapa_actual = NIVEL1;
                    cambiar_musica(recursos, recursos->musicas[1]);
                    break;

                case 1:
                    *nivel_actual = 2;
                    *etapa_actual = NIVEL2;
                    cambiar_musica(recursos, recursos->musicas[1]);
                    break;

                case 2:
                    *nivel_actual = 3;
                    *etapa_actual = NIVEL3;
                    cambiar_musica(recursos, recursos->musicas[1]);
                    break;

                case 3:
                    *nivel_actual = 4;
                    *etapa_actual = NIVEL4;
                    cambiar_musica(recursos, recursos->musicas[2]);
                    break;

                case 4:
                    *nivel_actual = 5;
                    *etapa_actual = NIVEL5;
                    cambiar_musica(recursos, recursos->musicas[2]);
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


/**
 * Dibuja un rectángulo dentro de otro rectángulo ya definido previamente.
 * @param rect_destino Es el rectágulo sobre el cual se dibujará el otro rectángulo (que será el de origen, que deberá ser más pequeño que éste).
 * @param alto Es el alto en px que tendrá el rectángulo que se quiere dibujar.
 * @param ancho Es el ancho en px que tendrá el rectángulo que se quiere dibujar.
 * @param porcentaje_x Es el porcentaje medido horizontalmente en el cual estará centrado el rectángulo respecto del de destino.
 * @param porcentaje_y Es el porcentaje medido verticalmente en el cual estará centrado el rectángulo respexto del de destino.
 * @param relleno Es un booleano que indica si el rectángulo se desea que sea relleno o no.
 * @param color Color del cual se quiere dibujar el rectángulo.
 */
Rectangulo dibujar_rectangulo_en_rectangulo(Rectangulo rect_destino, float alto, float ancho, float porcentaje_x, float porcentaje_y, bool relleno, ALLEGRO_COLOR color)
{
    Rectangulo rect;

    rect.pos_inicial.x = rect_destino.pos_inicial.x + porcentaje_x / 100 * (rect_destino.pos_final.x - rect_destino.pos_inicial.x) - ancho / 2;
    rect.pos_inicial.y = rect_destino.pos_inicial.y + porcentaje_y / 100 * (rect_destino.pos_final.y - rect_destino.pos_inicial.y) - alto / 2;

    rect.pos_final.x = rect_destino.pos_inicial.x + porcentaje_x / 100 * (rect_destino.pos_final.x - rect_destino.pos_inicial.x) + ancho / 2;
    rect.pos_final.y = rect_destino.pos_inicial.y + porcentaje_y / 100 * (rect_destino.pos_final.y - rect_destino.pos_inicial.y) + alto / 2;
    
    if (relleno)
    {
        al_draw_filled_rectangle(rect.pos_inicial.x, rect.pos_inicial.y, rect.pos_final.x, rect.pos_final.y, color);
    }

    else
    {
        al_draw_rectangle(rect.pos_inicial.x, rect.pos_inicial.y, rect.pos_final.x, rect.pos_final.y, color, 2.0);
    }

    return rect;
}


Procedure mostrar_pantalla_datos(Personaje personaje, ALLEGRO_BITMAP* vida, ALLEGRO_FONT* fuente, ALLEGRO_FONT* fuente_sec, Natural nivel_actual)
{
    Natural i;
    Natural alto_linea, alto_barras;
    Rectangulo rectangulo_datos;
    Rectangulo rectangulo_subvidas;
    extern Natural puntuacion;
    char texto_nro_nivel[6] = {'\0'};
    char texto_nro_vidas[5] = {'\0'};
    char texto_puntuacion[5] = {'\0'};

    rectangulo_datos.pos_inicial.x = 0;
    rectangulo_datos.pos_inicial.y = (8./9) * ALTO_VENTANA;
    rectangulo_datos.pos_final.x = ANCHO_VENTANA;
    rectangulo_datos.pos_final.y = ALTO_VENTANA;

    alto_linea = rectangulo_datos.pos_final.y - rectangulo_datos.pos_inicial.y;

    dibujar_rectangulo(rectangulo_datos, GRIS);
    sprintf(texto_nro_nivel, "Lvl %hu", nivel_actual);
    dibujar_texto_en_rectangulo(texto_nro_nivel, rectangulo_datos, 5.0, 50.0, fuente);
    dibujar_rectangulo_en_rectangulo(rectangulo_datos, alto_linea, 0, 10.0, 50.0, false, NEGRO);  // Esto es equivalente a dibujar una línea (una de las dimensiones es 0)
    dibujar_imagen_en_rectangulo(vida, rectangulo_datos, 12.5, 35.0);  // Se dibuja el corazón en un 5% en x y un 50% en y del rectángulo
    sprintf(texto_nro_vidas, "× %hu", personaje.nro_vidas);
    dibujar_texto_en_rectangulo(texto_nro_vidas, rectangulo_datos, 18.0, 35.0, fuente);
    dibujar_texto_en_rectangulo("VIDAS", rectangulo_datos, 16.3, 75.0, fuente_sec);
    dibujar_rectangulo_en_rectangulo(rectangulo_datos, alto_linea, 0, 22.5, 50.0, false, NEGRO);
    rectangulo_subvidas = dibujar_rectangulo_en_rectangulo(rectangulo_datos, 50, 800, 50.0, 35.0, false, NEGRO);
    dibujar_texto_en_rectangulo("BARRA DE VIDA", rectangulo_datos, 50.0, 75.0, fuente_sec);

    alto_barras = 0.85 * (rectangulo_subvidas.pos_final.y - rectangulo_subvidas.pos_inicial.y);

    for (i=0; i<personaje.subvida_actual; i++)
    {
        dibujar_rectangulo_en_rectangulo(rectangulo_subvidas, alto_barras, 6.0, 0.625 + 99.375 / 100 * i, 50.0, true, VERDE_OSCURO);
    }

    dibujar_rectangulo_en_rectangulo(rectangulo_datos, alto_linea, 0, 78.0, 50.0, false, NEGRO);
    dibujar_texto_en_rectangulo("PUNTUACIÓN", rectangulo_datos, 84.0, 75.0, fuente_sec);
    sprintf(texto_puntuacion, "%hu", puntuacion);
    dibujar_texto_en_rectangulo(texto_puntuacion, rectangulo_datos, 84.0, 35.0, fuente);
    dibujar_rectangulo_en_rectangulo(rectangulo_datos, alto_linea, 0, 90.0, 50.0, false, NEGRO);   
}


Procedure mostrar_fondo_nivel(Imagen fondos[NRO_NIVELES], Natural nivel_actual, Natural iteracion)
{
    if (nivel_actual == 1)  // Nivel 1 no tiene fondo
    {
        determinar_color_pantalla(iteracion);
    }

    else if (nivel_actual == 3)  // Nivel en el cual tengo pensado añadir scroll (Por eso se está dibujando solo una parte de la imagen)
    {
        al_draw_scaled_bitmap(fondos[NIVEL3], 0, 0, ANCHO_VENTANA, 8./9*ALTO_VENTANA, 0, 0, 
                              al_get_bitmap_width(fondos[NIVEL3]), al_get_bitmap_height(fondos[NIVEL3]), 0);
    }

    else
    {
        al_draw_scaled_bitmap(fondos[nivel_actual-1], 0, 0, al_get_bitmap_width(fondos[nivel_actual-1]), 
                              al_get_bitmap_height(fondos[nivel_actual-1]), 0, 0, ANCHO_VENTANA, ALTO_JUEGO, 0);        
    }
}