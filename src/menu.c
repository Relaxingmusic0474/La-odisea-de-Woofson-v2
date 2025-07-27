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
        if (posicion_mouse.x >= menu.opciones[i].rectangulo.pos_inicial.x && posicion_mouse.x <= menu.opciones[i].rectangulo.pos_final.x &&
            posicion_mouse.y >= menu.opciones[i].rectangulo.pos_inicial.y && posicion_mouse.y <= menu.opciones[i].rectangulo.pos_final.y)
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

/*
bool inicializar_menu_principal(Menu* menu, ALLEGRO_FONT* fuente)
{
    Natural i;
    char* textos_opciones_menu[] = {"JUGAR", "INSTRUCCIONES", "RANKING"};

    menu->nro_opciones = sizeof(textos_opciones_menu) / sizeof(textos_opciones_menu[0]);

    menu->opcion_en_hover = -1;

    menu->opciones = (Boton *) malloc(menu->nro_opciones * sizeof(Boton));

    if (!menu->opciones)
    {
        printf("Error de asignacion de memoria al inicializar el menu principal.\n");
        return false;
    }

    for (i=0; i<menu->nro_opciones; i++)
    {
        menu->opciones[i].rectangulo.pos_inicial = (Vector) {ANCHO_VENTANA*0.55, ALTO_VENTANA*(0.50 + 0.15*i)};
        menu->opciones[i].rectangulo.pos_final = (Vector) {ANCHO_VENTANA*0.80, ALTO_VENTANA*(0.62 + 0.15*i)};
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
    char* textos_opciones_menu[] = {"1", "2", "3", "4", "5", "Volver atrás"}; 

    menu->nro_opciones = sizeof(textos_opciones_menu) / sizeof(textos_opciones_menu[0]);

    menu->opcion_en_hover = -1;

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

    for (i=0; i<NRO_NIVELES; i++)
    {
        menu->opciones[i].rectangulo.pos_inicial = (Vector) {ANCHO_VENTANA*(0.15 + 0.15*i), ALTO_VENTANA*0.44};
        menu->opciones[i].rectangulo.pos_final = (Vector) {ANCHO_VENTANA*(0.25 + 0.15*i), ALTO_VENTANA*0.56};
    }

    menu->opciones[i].rectangulo.pos_inicial = (Vector) {ANCHO_VENTANA*0.05, ALTO_VENTANA*0.80};
    menu->opciones[i].rectangulo.pos_final = (Vector) {ANCHO_VENTANA*0.20, ALTO_VENTANA*0.90};

    menu->fondo = al_load_bitmap("assets/images/menu_niveles.png");

    if (!menu->fondo)
    {
        printf("Error al cargar el fondo del menu de niveles.\n");
        finalizar_menu(menu);
        return false;
    }

    menu->fuente = fuente;
    menu->fuente_sec = NULL;
    menu->finalizado = false;
    printf("El menu de niveles ya esta inicializado.\n");

    return true;
}


bool inicializar_menu_derrota(Menu* menu, ALLEGRO_FONT* fuente_primaria, ALLEGRO_FONT* fuente_secundaria)
{
    Natural i;
    float ancho, alto, x0, x1, y0, y1;
    char* textos_opciones_menu[] = {"Reintentar", "Volver al menú principal"};

    menu->nro_opciones = sizeof(textos_opciones_menu) / sizeof(textos_opciones_menu[0]);

    menu->opcion_en_hover = -1;

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

    x0 = RECTANGULO_MENU_RESULTADO.pos_inicial.x;
    x1 = RECTANGULO_MENU_RESULTADO.pos_final.x;

    y0 = RECTANGULO_MENU_RESULTADO.pos_inicial.y;
    y1 = RECTANGULO_MENU_RESULTADO.pos_final.y;

    ancho = x1 - x0;
    alto = y1 - y0;

    for (i=0; i<menu->opciones; i++) 
    {
        menu->opciones[i].rectangulo.pos_inicial = (Vector) {x0 + ancho*(0.20 + 0.35*i), y0 + alto*0.60};
        menu->opciones[i].rectangulo.pos_final = (Vector) {x0 + ancho*(0.45 + 0.35*i), y0 + alto*0.80};
    }


}
*/


bool inicializar_menu(Menu* menu, TipoMenu tipo, Imagen fondo, ALLEGRO_FONT* fuente, ALLEGRO_FONT* fuente_sec, char* textos_opciones[], Natural nro_opciones, Rectangulo rect_destino)
{
    Natural i;
    float x0, x1, y0, y1, ancho, alto;
    char* tipos_menu[] = {"principal", "de niveles", "de derrota", "de victoria"};

    menu->tipo = tipo;
    menu->nro_opciones = nro_opciones;
    menu->opcion_en_hover = -1;

    menu->opciones = (Boton *) malloc(menu->nro_opciones * sizeof(Boton));

    if (!menu->opciones)
    {
        printf("Error de asignacion de memoria al inicializar el menu %s.\n", tipos_menu[menu->tipo]);
        return false;
    }

    for (i=0; i<menu->nro_opciones; i++)
    {
        strcpy(menu->opciones[i].texto, textos_opciones[i]);
    }

    x0 = rect_destino.pos_inicial.x;
    x1 = rect_destino.pos_final.x;

    y0 = rect_destino.pos_inicial.y;
    y1 = rect_destino.pos_final.y;

    ancho = x1 - x0;
    alto = y1 - y0;

    if (menu->tipo == PRINCIPAL)  // Menú principal
    {
        for (i=0; i<menu->nro_opciones; i++)
        {
            menu->opciones[i].rectangulo.pos_inicial = (Vector) {x0 + ancho*0.55, y0 + alto*(0.50 + 0.15*i)};
            menu->opciones[i].rectangulo.pos_final = (Vector) {x0 + ancho*0.80, y0 + alto*(0.62 + 0.15*i)};
        }
    }

    else if (menu->tipo == NIVELES)  // Menú de niveles
    {
        for (i=0; i<NRO_NIVELES; i++)  // Asignación de coordenadas para los botones de los niveles
        {
            menu->opciones[i].rectangulo.pos_inicial = (Vector) {x0 + ancho*(0.15 + 0.15*i), y0 + alto*0.44};
            menu->opciones[i].rectangulo.pos_final = (Vector) {x0 + ancho*(0.25 + 0.15*i), y0 + alto*0.56};
        }

        // Coordenadas para el botón "Volver atrás"
        menu->opciones[i].rectangulo.pos_inicial = (Vector) {x0 + ancho*0.05, y0 + alto*0.80};
        menu->opciones[i].rectangulo.pos_final = (Vector) {x0 + ancho*0.20, y0 + alto*0.90};
    }

    else if (menu->tipo == PERDER)  // Menú que se muestra al perder un nivel
    {
        for (i=0; i<menu->nro_opciones; i++)
        {
            menu->opciones[i].rectangulo.pos_inicial = (Vector) {x0 + ancho*(0.20 + 0.35*i), y0 + alto*0.60};
            menu->opciones[i].rectangulo.pos_final = (Vector) {x0 + ancho*(0.45 + 0.35*i), y0 + alto*0.80};
        }
    }

    else  // Menú que se muestra al ganar un nivel
    {
        for (i=0; i<menu->nro_opciones; i++)
        {
            menu->opciones[i].rectangulo.pos_inicial = (Vector) {x0 + ancho*(0.15 + 0.26*i), y0 + alto*0.60};
            menu->opciones[i].rectangulo.pos_final = (Vector) {x0 + ancho*(0.33 + 0.26*i), y0 + alto*0.80};
        }
    }

    menu->rect_destino = rect_destino;
    menu->fondo = fondo;
    menu->fuente = fuente;
    menu->fuente_sec = fuente_sec;
    menu->finalizado = false;
    return true;
}


Procedure mostrar_menu(Menu menu)
{
    Rectangulo rect, rect_menu;
    float alto, ancho, x0, x1, y0, y1, porcentaje_x, porcentaje_y;
    extern Natural puntuacion;
    char texto_puntuacion[LARGO] = {'\0'};
    Natural i;

    menu.opcion_en_hover = obtener_opcion_en_hover(menu);  // Se obtiene la opcion por la que pasa el cursor (sin seleccionarla aun)
    
    x0 = menu.rect_destino.pos_inicial.x;
    x1 = menu.rect_destino.pos_final.x;

    y0 = menu.rect_destino.pos_inicial.y;
    y1 = menu.rect_destino.pos_final.y;

    ancho = x1 - x0;
    alto = y1 - y0;

    // Aqui se muestra el fondo del menu (si es que no es NULL)
    if (menu.fondo != NULL)
    {
        al_draw_scaled_bitmap(menu.fondo, 0, 0, al_get_bitmap_width(menu.fondo), al_get_bitmap_height(menu.fondo), (ANCHO_VENTANA-ancho)/2, (ALTO_VENTANA-alto)/2, ancho, alto, 0);
    }

    else  // Si el menú que se quiere mostrar, no tiene fondo (es decir, es el menú de ganar o perder), se dibuja el menú como un rectángulo
    {
        rect_menu = dibujar_rectangulo_en_rectangulo(RECTANGULO_JUEGO, alto, ancho, 50.0, 50.0, true, CAFE);
        dibujar_rectangulo_en_rectangulo(rect_menu, alto, ancho, 50.0, 50.0, false, AMARILLO);
    }

    if (menu.tipo == NIVELES)  // Si es el menú de niveles, hace esto
    {
        al_draw_filled_rectangle(x0 + ancho*0.10, y0 + alto*0.30, x0 + ancho*0.90, y0 + alto*0.70, GRIS);
        al_draw_filled_rectangle(x0 + ancho*0.30, y0 + alto*0.22, x0 + ancho*0.70, y0 + alto*0.38, AZUL);
        al_draw_text(menu.fuente, BLANCO, x0 + ancho*0.50, y0 + alto*0.30, ALLEGRO_ALIGN_CENTRE, "SELECCIONE UN NIVEL:");
    }

    else
    {
        if (menu.tipo == PERDER)  // Si el menú es el menú de derrota
        {
            dibujar_texto_en_rectangulo("HAS PERDIDO", menu.rect_destino, 50.0, 15.0, menu.fuente, BLANCO);
            sprintf(texto_puntuacion, "Su puntuación: %hu", puntuacion);
            dibujar_texto_en_rectangulo(texto_puntuacion, menu.rect_destino, 50.0, 40.0, menu.fuente_sec ? menu.fuente_sec : menu.fuente, BLANCO);
        }

        else
        {
            if (menu.tipo == GANAR)
            {
                dibujar_texto_en_rectangulo("FELICIDADES... HAS LOGRADO PASAR EL NIVEL", menu.rect_destino, 50.0, 15.0, menu.fuente, BLANCO);
                sprintf(texto_puntuacion, "Su puntuación: %hu", puntuacion);
                dibujar_texto_en_rectangulo(texto_puntuacion, menu.rect_destino, 50.0, 40.0, menu.fuente_sec ? menu.fuente_sec : menu.fuente, BLANCO);
            }
        }
    }

    for (i=0; i<menu.nro_opciones; i++)
    {
        alto = menu.opciones[i].rectangulo.pos_final.y - menu.opciones[i].rectangulo.pos_inicial.y;
        ancho = menu.opciones[i].rectangulo.pos_final.x - menu.opciones[i].rectangulo.pos_inicial.x;

        porcentaje_x = (menu.opciones[i].rectangulo.pos_inicial.x + ancho/2)/ANCHO_VENTANA * 100;
        porcentaje_y = (menu.opciones[i].rectangulo.pos_inicial.y + alto/2)/ALTO_VENTANA * 100;

        rect = dibujar_rectangulo_en_rectangulo(RECTANGULO_VENTANA, alto, ancho, porcentaje_x, porcentaje_y, false, menu.opcion_en_hover == i ? AZUL : NEGRO);
        dibujar_texto_en_rectangulo(menu.opciones[i].texto, rect, 50.0, 50.0, menu.fuente, menu.opcion_en_hover == i ? AZUL : NEGRO);
    }

    al_flip_display();  // Se muestra el menu

    return;
}


Procedure cambiar_menu(Menu* menu_actual, Menu menu_nuevo)
{
    *menu_actual = menu_nuevo;
}


Procedure redirigir_menu(Recursos* recursos, Natural opcion_clickeada, Etapa* etapa_actual, Natural* nivel_actual)
{
    if (*etapa_actual == MENU_PRINCIPAL)
    {
        *nivel_actual = 0;

        if (opcion_clickeada == 0)  
        {
            *etapa_actual = MENU_NIVELES;
            cambiar_menu(&recursos->menu_actual, recursos->menus[MENU_NIVELES+4]);  // +4 para que calze con el índice
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
            cambiar_menu(&recursos->menu_actual, recursos->menus[MENU_PRINCIPAL+4]);
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

    else
    {
        if (*etapa_actual == DERROTA)
        {
            cambiar_menu(&recursos->menu_actual, recursos->menus[PERDER]);
            mostrar_menu(recursos->menu_actual);
        }

        else if (*etapa_actual == VICTORIA)
        {
            cambiar_menu(&recursos->menu_actual, recursos->menus[GANAR]);
            mostrar_menu(recursos->menu_actual);
        }
    }

    return;
}


Procedure finalizar_menu(Menu* menu)
{   
    Natural i;

    for (i=0; i<menu->nro_opciones; i++)
    {
        memset(menu->opciones[i].texto, '\0', sizeof(menu->opciones[i].texto));
        menu->opciones[i].rectangulo = (Rectangulo) {VECTOR_NULO, VECTOR_NULO};
    }

    menu->fuente_sec = NULL;
    menu->fuente = NULL;

    if (menu->opciones != NULL)
    {
        free(menu->opciones);
        menu->opciones = NULL;
    }

    menu->fondo = NULL;
    menu->rect_destino = (Rectangulo) {VECTOR_NULO, VECTOR_NULO};
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


Procedure dibujar_texto_en_rectangulo(char* texto, Rectangulo rectangulo, float porcentaje_x, float porcentaje_y, ALLEGRO_FONT* fuente, ALLEGRO_COLOR color)
{
    Vector posicion_texto;
    int alto_texto = al_get_font_line_height(fuente);
    posicion_texto.x = rectangulo.pos_inicial.x + (float) porcentaje_x / 100 * (rectangulo.pos_final.x - rectangulo.pos_inicial.x);
    posicion_texto.y = rectangulo.pos_inicial.y + (float) porcentaje_y / 100 * (rectangulo.pos_final.y - rectangulo.pos_inicial.y);
    al_draw_text(fuente, color, posicion_texto.x, posicion_texto.y - (float) alto_texto / 2, ALLEGRO_ALIGN_CENTRE, texto);
}


/**
 * @brief Función que obtiene datos de un rectángulo, pero sin dibujarlo.
 * @param rect_destino Es el rectágulo sobre el cual estará el otro rectángulo (que será el de origen, que deberá ser más pequeño que éste).
 * @param alto Es el alto en px que tendrá el rectángulo.
 * @param ancho Es el ancho en px que tendrá el rectángulo.
 * @param porcentaje_x Es el porcentaje medido horizontalmente en el cual estará centrado el rectángulo respecto del de destino.
 * @param porcentaje_y Es el porcentaje medido verticalmente en el cual estará centrado el rectángulo respexto del de destino.
 */
Rectangulo obtener_rectangulo(Rectangulo rect_destino, float alto, float ancho, float porcentaje_x, float porcentaje_y)
{
    Rectangulo rect;

    rect.pos_inicial.x = rect_destino.pos_inicial.x + porcentaje_x / 100 * (rect_destino.pos_final.x - rect_destino.pos_inicial.x) - ancho / 2;
    rect.pos_inicial.y = rect_destino.pos_inicial.y + porcentaje_y / 100 * (rect_destino.pos_final.y - rect_destino.pos_inicial.y) - alto / 2;

    rect.pos_final.x = rect_destino.pos_inicial.x + porcentaje_x / 100 * (rect_destino.pos_final.x - rect_destino.pos_inicial.x) + ancho / 2;
    rect.pos_final.y = rect_destino.pos_inicial.y + porcentaje_y / 100 * (rect_destino.pos_final.y - rect_destino.pos_inicial.y) + alto / 2;

    return rect;
}


/**
 * @brief Dibuja un rectángulo dentro de otro rectángulo ya definido previamente.
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

    rect = obtener_rectangulo(rect_destino, alto, ancho, porcentaje_x, porcentaje_y);
    
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
    rectangulo_datos.pos_inicial.y = ALTO_JUEGO;
    rectangulo_datos.pos_final.x = ANCHO_VENTANA;
    rectangulo_datos.pos_final.y = ALTO_VENTANA;

    alto_linea = rectangulo_datos.pos_final.y - rectangulo_datos.pos_inicial.y;

    dibujar_rectangulo(rectangulo_datos, GRIS);
    sprintf(texto_nro_nivel, "Lvl %hu", nivel_actual);
    dibujar_texto_en_rectangulo(texto_nro_nivel, rectangulo_datos, 5.0, 50.0, fuente, NEGRO);
    dibujar_rectangulo_en_rectangulo(rectangulo_datos, alto_linea, 0, 10.0, 50.0, false, NEGRO);  // Esto es equivalente a dibujar una línea (una de las dimensiones es 0)
    dibujar_imagen_en_rectangulo(vida, rectangulo_datos, 12.5, 35.0);  // Se dibuja el corazón en un 5% en x y un 50% en y del rectángulo
    sprintf(texto_nro_vidas, "× %hu", personaje.nro_vidas);
    dibujar_texto_en_rectangulo(texto_nro_vidas, rectangulo_datos, 18.0, 35.0, fuente, NEGRO);
    dibujar_texto_en_rectangulo("VIDAS", rectangulo_datos, 16.3, 75.0, fuente_sec, NEGRO);
    dibujar_rectangulo_en_rectangulo(rectangulo_datos, alto_linea, 0, 22.5, 50.0, false, NEGRO);
    rectangulo_subvidas = dibujar_rectangulo_en_rectangulo(rectangulo_datos, 50, 800, 50.0, 35.0, false, NEGRO);
    dibujar_texto_en_rectangulo("BARRA DE VIDA", rectangulo_datos, 50.0, 75.0, fuente_sec, NEGRO);

    alto_barras = 0.85 * (rectangulo_subvidas.pos_final.y - rectangulo_subvidas.pos_inicial.y);

    for (i=0; i<personaje.subvida_actual; i++)
    {
        dibujar_rectangulo_en_rectangulo(rectangulo_subvidas, alto_barras, 6.0, 0.625 + 99.375 / 100 * i, 50.0, true, VERDE_OSCURO);
    }

    dibujar_rectangulo_en_rectangulo(rectangulo_datos, alto_linea, 0, 78.0, 50.0, false, NEGRO);
    dibujar_texto_en_rectangulo("PUNTUACIÓN", rectangulo_datos, 84.0, 75.0, fuente_sec, NEGRO);
    sprintf(texto_puntuacion, "%hu", puntuacion);
    dibujar_texto_en_rectangulo(texto_puntuacion, rectangulo_datos, 84.0, 35.0, fuente, NEGRO);
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
        if (nivel_actual != 0)
        {
            al_draw_scaled_bitmap(fondos[nivel_actual-1], 0, 0, al_get_bitmap_width(fondos[nivel_actual-1]), 
                                  al_get_bitmap_height(fondos[nivel_actual-1]), 0, 0, ANCHO_VENTANA, ALTO_JUEGO, 0);
        }        
    }
}