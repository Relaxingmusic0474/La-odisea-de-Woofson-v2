#include "menu.h"

/**
 * @brief Función que obtiene la posición del mouse.
 * @return Un vector 2D que representa la posición actual del cursor.
 */
Vector obtener_posicion_mouse()
{
    ALLEGRO_MOUSE_STATE estado_mouse;
    Vector posicion_mouse;

    al_get_mouse_state(&estado_mouse);

    posicion_mouse.x = estado_mouse.x;
    posicion_mouse.y = estado_mouse.y;

    return posicion_mouse;
}


/**
 * @brief Función que obtiene la opción sobrevolada (número entre 0 y N-1) en un menú de N opciones.
 * @param menu Es el menú del cual se desea saber la opción que es sobrevolada con el cursor.
 * @return Un número natural entre 0 y N-1 que representa el identificador de la opción sobrevolada (si es que hay alguna), y -1 en caso contrario.
 */
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
 * @brief Función que determina el color de la pantalla según la iteración del juego (se utiliza en el primer nivel).
 * @param iteracion Es el número de iteración del juego, que se incrementa cada frame.
 */
Procedure determinar_color_pantalla(Natural iteracion)
{
    ALLEGRO_COLOR lista_colores[NRO_COLORES] = {AZUL, ROSADO, MORADO, VERDE_OSCURO, ROJO};
    static Natural i = 0;  // Estática para que ignore el valor inicial de la segunda llamada en adelante
    ALLEGRO_COLOR color = lista_colores[i];

    if (iteracion % 30 == 0)
    {
        color = lista_colores[i];
        i = (i + 1) % NRO_COLORES;
        
    }

    al_clear_to_color(color);
}


/**
 * @brief Función que inicializa un determinado menú.
 * @param menu Es el puntero al menú que se desea inicializar (puntero para que se pueda inicializar o modificar sus parámetros).
 * @param tipo Es el tipo de menú (enum) que se desea inicializar.
 * @param fondo Es la imagen de fondo que tendrá el menú (si no se quiere fondo, se pasa NULL).
 * @param fuente Es la fuente principal con la se escribirá el menú.
 * @param fuente_sec Es la fuente secundaria que se usará para el menú (no siempre se usan dos fuentes, así que si se usa solo una, este parámetro es NULL).
 * @param texto_opciones Es un arreglo de cadenas que representa el texto que irá en cada una de las opciones del menú.
 * @param rect_destino Es el rectángulo sobre el cuál se implementará el menú (usualmente ocupa toda la ventana).
 * @return true si se inicializa correctamente el menú, false si hay algún error en su inicialización (usualmente debido a errores de asignación de memoria).
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
        menu->opciones[i].rectangulo.pos_inicial = (Vector) {x0 + ancho*0.05, y0 + alto*0.83};
        menu->opciones[i].rectangulo.pos_final = (Vector) {x0 + ancho*0.20, y0 + alto*0.92};
    }

    else if (menu->tipo == PERDER)  // Menú que se muestra al perder un nivel
    {
        for (i=0; i<menu->nro_opciones; i++)
        {
            menu->opciones[i].rectangulo.pos_inicial = (Vector) {x0 + ancho*(0.20 + 0.35*i), y0 + alto*0.60};
            menu->opciones[i].rectangulo.pos_final = (Vector) {x0 + ancho*(0.45 + 0.35*i), y0 + alto*0.80};
        }
    }

    else if (menu->tipo == GANAR)  // Menú que se muestra al ganar un nivel
    {
        for (i=0; i<menu->nro_opciones; i++)
        {
            menu->opciones[i].rectangulo.pos_inicial = (Vector) {x0 + ancho*(0.15 + 0.26*i), y0 + alto*0.60};
            menu->opciones[i].rectangulo.pos_final = (Vector) {x0 + ancho*(0.33 + 0.26*i), y0 + alto*0.80};
        }
    }

    else  // Menú que funciona como un ranking (Solo tiene la opción de Volver atrás)
    {    
        menu->opciones[0].rectangulo.pos_inicial = (Vector) {x0 + ancho*0.05, y0 + alto*0.83};
        menu->opciones[0].rectangulo.pos_final = (Vector) {x0 + ancho*0.20, y0 + alto*0.92};
    }

    menu->rect_destino = rect_destino;
    menu->fondo = fondo;
    menu->fuente = fuente;
    menu->fuente_sec = fuente_sec;
    menu->finalizado = false;
    return true;
}


/**
 * @brief Función que muestra un cierto menú de acuerdo también a la etapa del juego.
 * @param menu Es el menú que se desea mostrar (lo lógico es que se muestre el menú actual).
 * @param etapa_actual Es la etapa actual del juego (enum).
 */
Procedure mostrar_menu(Menu menu, Etapa etapa_actual)
{
    Rectangulo rect, rect_menu;
    Menu menu_vacio = {0};
    float alto, ancho, x0, x1, y0, y1, porcentaje_x, porcentaje_y;
    // extern Natural puntuacion;
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
        if (memcmp(&menu, &menu_vacio, sizeof(Menu)) != 0)
        {
            if (menu.tipo != NIVELES || (menu.tipo == NIVELES && etapa_actual != RANKING))
            {
                al_draw_scaled_bitmap(menu.fondo, 0, 0, al_get_bitmap_width(menu.fondo), al_get_bitmap_height(menu.fondo), (ANCHO_VENTANA-ancho)/2, (ALTO_VENTANA-alto)/2, ancho, alto, 0);
            }
        }
    }

    else  // Si el menú que se quiere mostrar, no tiene fondo (es decir, es el menú de ganar o perder, o de ranking), se dibuja el menú como un rectángulo
    {
        if (menu.tipo != MENU_RANK)
        {
            if (memcmp(&menu, &menu_vacio, sizeof(Menu)) != 0)
            {
                rect_menu = dibujar_rectangulo_en_rectangulo(RECTANGULO_JUEGO, alto, ancho, 50.0, 50.0, true, CAFE);
                dibujar_rectangulo_en_rectangulo(rect_menu, alto, ancho, 50.0, 50.0, false, AMARILLO);
            }
        }
    }

    if (memcmp(&menu, &menu_vacio, sizeof(Menu)) != 0 && menu.tipo == NIVELES)  // Si es el menú de niveles, hace esto
    {
        al_draw_filled_rectangle(x0 + ancho*0.10, y0 + alto*0.30, x0 + ancho*0.90, y0 + alto*0.70, GRIS);
        al_draw_filled_rectangle(x0 + ancho*0.30, y0 + alto*0.22, x0 + ancho*0.70, y0 + alto*0.38, AZUL);
        al_draw_text(menu.fuente, BLANCO, x0 + ancho*0.50, y0 + alto*0.30, ALLEGRO_ALIGN_CENTRE, etapa_actual == MENU_NIVELES ? "SELECCIONE UN NIVEL:" : "SELECCIONE EL RANKING QUE DESEA VER:");
    }

    else
    {
        if (memcmp(&menu, &menu_vacio, sizeof(Menu)) != 0)  // Si el menú es el menú de derrota
        {
            if (etapa_actual == DERROTA)
            {
                dibujar_texto_en_rectangulo("HAS PERDIDO", menu.rect_destino, 50.0, 15.0, menu.fuente, BLANCO);
                sprintf(texto_puntuacion, "Su puntuación: %hu", puntuacion);
                dibujar_texto_en_rectangulo(texto_puntuacion, menu.rect_destino, 50.0, 40.0, menu.fuente_sec ? menu.fuente_sec : menu.fuente, BLANCO);
            }

            else
            {
                if (etapa_actual == VICTORIA)
                {
                    dibujar_texto_en_rectangulo("FELICIDADES... HAS LOGRADO PASAR EL NIVEL", menu.rect_destino, 50.0, 15.0, menu.fuente, BLANCO);
                    sprintf(texto_puntuacion, "Su puntuación: %hu", puntuacion);
                    dibujar_texto_en_rectangulo(texto_puntuacion, menu.rect_destino, 50.0, 40.0, menu.fuente_sec ? menu.fuente_sec : menu.fuente, BLANCO);
                }
            }
        }
    }

    for (i=0; i<menu.nro_opciones; i++)
    {
        alto = menu.opciones[i].rectangulo.pos_final.y - menu.opciones[i].rectangulo.pos_inicial.y;
        ancho = menu.opciones[i].rectangulo.pos_final.x - menu.opciones[i].rectangulo.pos_inicial.x;

        porcentaje_x = (menu.opciones[i].rectangulo.pos_inicial.x + ancho/2)/ANCHO_VENTANA * 100;
        porcentaje_y = (menu.opciones[i].rectangulo.pos_inicial.y + alto/2)/ALTO_VENTANA * 100;

        if (menu.tipo == PRINCIPAL || menu.tipo == NIVELES)
        {
            rect = dibujar_rectangulo_en_rectangulo(RECTANGULO_VENTANA, alto, ancho, porcentaje_x, porcentaje_y, false, menu.opcion_en_hover == i ? AZUL : NEGRO);
            dibujar_texto_en_rectangulo(menu.opciones[i].texto, rect, 50.0, 50.0, menu.fuente, menu.opcion_en_hover == i ? AZUL : NEGRO);
        }

        else
        {
            rect = dibujar_rectangulo_en_rectangulo(RECTANGULO_VENTANA, alto, ancho, porcentaje_x, porcentaje_y, false, menu.opcion_en_hover == i ? AZUL : BLANCO);
            dibujar_texto_en_rectangulo(menu.opciones[i].texto, rect, 50.0, 50.0, menu.fuente_sec, menu.opcion_en_hover == i ? AZUL : BLANCO);
        }
    }

    al_flip_display();  // Se muestra el menu

    return;
}


/**
 * @brief Función que muestra un determinado ranking de juego.
 * @param menu Un puntero al menú que se quiere mostrar (un ranking también es un menú solo que es especial).
 * @param ranking Es la estructura de ranking, que además incluye los datos de los jugadores y sus puntuaciones.
 */
Procedure mostrar_ranking(Menu* menu, Ranking* ranking)
{
    Natural i;
    Rectangulo datos[3][MAX_DATOS];
    float alto, sep, xm, ym, alto_fuente;
    char pos[5] = {'\0'};
    char pje[5] = {'\0'};

    menu->opcion_en_hover = obtener_opcion_en_hover(*menu);  // Se obtiene la opcion por la que pasa el cursor (sin seleccionarla aun)

    dibujar_rectangulo_en_rectangulo(RECTANGULO_VENTANA, ALTO_VENTANA, ANCHO_VENTANA, 50.0, 50.0, true, MORADO);
    dibujar_texto_en_rectangulo("RANKING", RECTANGULO_VENTANA, 50.0, 7.5, menu->fuente, BLANCO);

    alto = 0.5 / MAX_DATOS * ALTO_VENTANA;
    sep = alto / 4;

    for (i=0; i<MAX_DATOS; i++)
    {
        datos[0][i] = dibujar_rectangulo_en_rectangulo(RECTANGULO_VENTANA, alto, 0.08*ANCHO_VENTANA, 16.5, 17.0 + 100*(alto+sep)/ALTO_VENTANA*i, true, ROJO);
        dibujar_rectangulo_en_rectangulo(RECTANGULO_VENTANA, alto, 0.08*ANCHO_VENTANA, 16.5, 17.0 + 100*(alto+sep)/ALTO_VENTANA*i, false, AMARILLO);
        sprintf(pos, "%hu", i+1);
        dibujar_texto_en_rectangulo(pos, datos[0][i], 50.0, 50.0, menu->fuente, NEGRO);
        datos[1][i] = dibujar_rectangulo_en_rectangulo(RECTANGULO_VENTANA, alto, 0.49*ANCHO_VENTANA, 45.0, 17.0 + 100*(alto+sep)/ALTO_VENTANA*i, true, GRIS_SEMIOSCURO);
        dibujar_rectangulo_en_rectangulo(RECTANGULO_VENTANA, alto, 0.49*ANCHO_VENTANA, 45.0, 17.0 + 100*(alto+sep)/ALTO_VENTANA*i, false, AMARILLO);
        dibujar_texto_en_rectangulo(ranking->datos[i].nombre, datos[1][i], 50.0, 50.0, menu->fuente_sec, NEGRO);
        datos[2][i] = dibujar_rectangulo_en_rectangulo(RECTANGULO_VENTANA, alto, 0.18*ANCHO_VENTANA, 78.5, 17.0 + 100*(alto+sep)/ALTO_VENTANA*i, true, GRIS_SEMIOSCURO);
        dibujar_rectangulo_en_rectangulo(RECTANGULO_VENTANA, alto, 0.18*ANCHO_VENTANA, 78.5, 17.0 + 100*(alto+sep)/ALTO_VENTANA*i, false, AMARILLO);
        snprintf(pje, sizeof(pje), "%hu", ranking->datos[i].puntaje);
        dibujar_texto_en_rectangulo(pje, datos[2][i], 50.0, 50.0, menu->fuente_sec, NEGRO);
        memset(pos, '\0', sizeof(pos));
        memset(pje, '\0', sizeof(pje));
    }

    al_draw_rectangle(menu->opciones[0].rectangulo.pos_inicial.x, menu->opciones[0].rectangulo.pos_inicial.y, menu->opciones[0].rectangulo.pos_final.x,
                      menu->opciones[0].rectangulo.pos_final.y, menu->opcion_en_hover == 0 ? AZUL : BLANCO, 2.0);

    xm = (menu->opciones[0].rectangulo.pos_inicial.x + menu->opciones[0].rectangulo.pos_final.x) / 2;
    ym = (menu->opciones[0].rectangulo.pos_inicial.y + menu->opciones[0].rectangulo.pos_final.y) / 2;

    alto_fuente = al_get_font_line_height(menu->fuente_sec);

    al_draw_text(menu->fuente_sec, menu->opcion_en_hover == 0 ? AZUL : BLANCO, xm, (ym-alto_fuente/2), ALLEGRO_ALIGN_CENTRE, menu->opciones[0].texto);    

    al_flip_display();
}


/**
 * @brief Función que cambia el menú que se debe mostrar actualmente (se usa harto en la función redirigir_menu())
 * @param menu_actual Un puntero al menú actual (el que se estaba mostrando).
 * @param menu_nuevo El menú que se quiere mostrar ahora.
 */
Procedure cambiar_menu(Menu* menu_actual, Menu menu_nuevo)
{
    *menu_actual = menu_nuevo;
}


/**
 * @brief Función que cambia el tipo de munición de acuerdo al nivel.
 * @param municiones El arreglo con los objetos municiones.
 * @param municion_normal La imagen con la munición normal.
 * @param municion_pro La imagen con la munición pro.
 * @param nivel_actual El nivel actual en el que se está.
 */
Procedure cambiar_tipo_municion(Municion municiones[MAX_MUNICIONES], Imagen municion_normal, Imagen municion_pro, Natural nivel_actual)
{
    Natural i;

    for (i=0; i<MAX_MUNICIONES; i++)
    {
        if (nivel_actual >= 4)
        {
            municiones[i].imagen = municion_pro;
        }

        else
        {
            municiones[i].imagen = municion_normal;
        }
    }
}


/**
 * @brief Función que reinicia el estado del juego.
 * @param recursos Un puntero a la estructura con todos los recursos esenciales del juego.
 * @param menu El menú que se desea que se muestre.
 * @param etapa_actual Un puntero a la etapa actual del juego (a veces al resetear el estado del juego la etapa actual debe cambiar).
 * @param etapa_deseada La etapa a la que se desea cambiar.
 */
Procedure resetear_estado_juego(Recursos* recursos, Menu menu, Etapa* etapa_actual, Etapa etapa_deseada, float* tiempo_en_nivel)
{
    Natural i;

    cambiar_menu(&recursos->menu_actual, menu);
    inicializar_personaje(&recursos->pje_principal, WOOFSON, recursos->frames, (Vector){ANCHO_VENTANA * 0.1, ALTURA_PISO - al_get_bitmap_height(recursos->frames[FRAME_WOOFSON][0])}, false);
    teclas[ALLEGRO_KEY_RIGHT] = true;
    memset((bool *) teclas, false, sizeof(teclas));
    recursos->puerta.estado = CERRADA;
    recursos->palanca.estado = DESACTIVADA;
    puntuacion = PUNTUACION_INICIAL;
    desactivar_enemigos(recursos->enemigos);
    *etapa_actual = etapa_deseada;

    for (i=0; i<MAX_POCIONES; i++)
    {
        recursos->pociones[i].tomada = false;
    }

    for (i=0; i<MAX_POCIONES_RANGO_BALA; i++)
    {
        recursos->pociones_rango_bala[i].tomada = false;
    }

    for (i=0; i<MAX_MUNICIONES; i++)
    {
        recursos->municiones[i].tomada = false;
    }

    for (i=0; i<MAX_ESPINAS; i++)
    {
        recursos->espinas[i].activa = false;
    }

    for (i=0; i<MAX_CHARCOS; i++)
    {
        recursos->charcos[i].activo = false;
    }

    *tiempo_en_nivel = 0;
}


/**
 * @brief Función que detiene los efectos de sonido utilizados en el juego.
 * @param recursos Un puntero a la estructura recursos, que contiene los recursos esenciales para el juego (incluye los efectos de sonido).
 */
Procedure detener_efectos_de_sonido(Recursos* recursos)
{
    Natural i, j;

    for (i=0; i<NRO_NIVELES; i++)
    {
        for (j=0; j<NRO_INSTANCIAS;j++)
        {
            if (recursos->rayos[i][j].efecto_sonido_ya_empezado)
            {
                al_stop_sample_instance(recursos->rayos[i][j].efecto_sonido->instancias[j]);

                if (j==NRO_INSTANCIAS-1)
                {
                    recursos->rayos[i][j].efecto_sonido_ya_empezado = false;
                    recursos->rayos[i][j].efecto_sonido_ya_detenido = true;
                }
            }   
        }   
    }
}


/**
 * @brief Función que redirige el menú (cambia el menú actual) y además resetea y reconfigura parámetros de acuerdo al menú mostrado.
 * @param recursos Es un puntero a la estructura que contiene los recursos del juego.
 * @param opcion_clickeada Es el identificador de la opción que se clickea con el mouse (depende de eso la redirección será distinta).
 * @param etapa_actual Es el puntero a la etapa actual del juego (cambia al cambiar de menú).
 * @param nivel_actual Es el puntero al nivel actual del juego (en algunos casos cambia al cambiar de menú).
 * @param ranking_a_mostrar Es el puntero al identificador del ranking que se debe mostrar (si no se debe mostrar ninguno, entonces es -1).
 * @param tiempo_en_nivel Un puntero al tiempo que se lleva en el nivel actual.
 */
Procedure redirigir_menu(Recursos* recursos, Natural opcion_clickeada, Etapa* etapa_actual, Natural* nivel_actual, Natural* ranking_a_mostrar, float* tiempo_en_nivel)//, Natural* ranking_a_mostrar)
{
    Menu menu_vacio = {0};
    Natural nivel_anterior = 0;
    char nombre[LARGO] = {'\0'};  // Para entrar al ranking

    if (*etapa_actual == MENU_PRINCIPAL)
    {
        *nivel_actual = 0;

        if (opcion_clickeada == 0)  
        {
            *etapa_actual = MENU_NIVELES;
            cambiar_menu(&recursos->menu_actual, recursos->menus[NIVELES]);
        }

        else if (opcion_clickeada == 1)
        {
            *etapa_actual = INSTRUCCIONES;
            al_flip_display();
        }

        else
        {
            *etapa_actual = MENU_RANKING;
            cambiar_menu(&recursos->menu_actual, recursos->menus[NIVELES]);
        }
    }

    else if (*etapa_actual == MENU_NIVELES)
    {
        if (opcion_clickeada == 5)
        {
            *nivel_actual = 0;
            *etapa_actual = MENU_PRINCIPAL;
            detener_efectos_de_sonido(recursos);
            cambiar_menu(&recursos->menu_actual, recursos->menus[PRINCIPAL]);
        }

        else
        {
            cambiar_menu(&recursos->menu_actual, menu_vacio);
            inicializar_personaje(&recursos->pje_principal, WOOFSON, recursos->frames, (Vector) {ANCHO_VENTANA*0.1, ALTURA_PISO-al_get_bitmap_height(recursos->frames[FRAME_WOOFSON][0])}, false);
            memset((bool *) teclas, false, sizeof(teclas));

            *nivel_actual = opcion_clickeada+1;
            *etapa_actual = (Etapa) opcion_clickeada;

            switch (opcion_clickeada)
            {
                case 0:
                case 1:
                case 2:
                    cambiar_musica(recursos, recursos->musicas[1]);
                    recursos->pje_principal.bala_recargable = false;
                    break;

                case 3:
                case 4:
                    cambiar_musica(recursos, recursos->musicas[2]);
                    recursos->pje_principal.bala_recargable = opcion_clickeada == 4 ? true : false;
                    break;
            }
            
            al_flip_display();
        }
    }

    else if (*etapa_actual == MENU_RANKING)
    {
        if (opcion_clickeada == 5)
        {
            *nivel_actual = 0;
            *etapa_actual = MENU_PRINCIPAL;
            detener_efectos_de_sonido(recursos);
            cambiar_menu(&recursos->menu_actual, recursos->menus[PRINCIPAL]);
        }

        else
        {
            *etapa_actual = RANKING;
            cambiar_menu(&recursos->menu_actual, recursos->menus[MENU_RANK]);
            mostrar_ranking(&recursos->menu_actual, &recursos->rankings[opcion_clickeada]);
            *ranking_a_mostrar = opcion_clickeada;
        }
    }

    else if (*etapa_actual == RANKING)
    {
        if (opcion_clickeada == 0)
        {
            *etapa_actual = MENU_RANKING;   
            cambiar_menu(&recursos->menu_actual, recursos->menus[NIVELES]);
        }
    }

    else
    {
        if (*etapa_actual == DERROTA)
        {
            cambiar_menu(&recursos->menu_actual, recursos->menus[PERDER]);

            if (opcion_clickeada == 0)  // Lógica para REINTENTAR el nivel
            {
                resetear_estado_juego(recursos, menu_vacio, etapa_actual, *nivel_actual-1, tiempo_en_nivel);

                if (*nivel_actual == 5)
                {
                    recursos->pje_principal.bala_recargable = true;
                }

                al_set_sample_instance_position(recursos->musica_actual->instancia, 0); // Reinicia la música
                al_play_sample_instance(recursos->musica_actual->instancia);  
            }

            else
            {
                if (opcion_clickeada == 1)  // Logica para volver al MENÚ PRINCIPAL
                {
                    resetear_estado_juego(recursos, recursos->menus[PRINCIPAL], etapa_actual, MENU_PRINCIPAL, tiempo_en_nivel);
                    *nivel_actual = 0;
                    detener_efectos_de_sonido(recursos);
                    cambiar_musica(recursos, recursos->musicas[0]);
                }
            }
        }

        else if (*etapa_actual == VICTORIA)
        {
            cambiar_menu(&recursos->menu_actual, recursos->menus[GANAR]);

            if (opcion_clickeada == 0)  // Lógica para volver al menú principal
            {
                resetear_estado_juego(recursos, recursos->menus[PRINCIPAL], etapa_actual, MENU_PRINCIPAL, tiempo_en_nivel);
                *nivel_actual = 0;
                detener_efectos_de_sonido(recursos);
                cambiar_musica(recursos, recursos->musicas[0]);
            }

            else if (opcion_clickeada == 1)
            {
                // Se solicitan los datos para ingresarse en el ranking
                ingresar_nombre(nombre, sizeof(nombre), recursos->fuentes[COMFORTAA_LIGHT_GIGANTE], recursos->fuentes[TIMES_NEW_ROMAN_GRANDE], recursos->cola_eventos);
                insertar_en_ranking(&recursos->rankings[(*nivel_actual)-1], nombre, *nivel_actual);
                modificar_ranking(&recursos->rankings[(*nivel_actual)-1], *nivel_actual);
            }

            else
            {
                if (opcion_clickeada == 2)
                {
                    nivel_anterior = *nivel_actual;  
                    
                    if (*nivel_actual != NRO_NIVELES)
                    {
                        resetear_estado_juego(recursos, menu_vacio, etapa_actual, (*nivel_actual-1)+1, tiempo_en_nivel);
                        detener_efectos_de_sonido(recursos);
                        *nivel_actual = *nivel_actual + 1;
                    }
                    
                    if (*nivel_actual == 4)
                    {
                        cambiar_musica(recursos, recursos->musicas[2]);
                    }
                        
                    if (*nivel_actual == 5)
                    {   
                        recursos->pje_principal.bala_recargable = true;
                    }

                    if (nivel_anterior != NRO_NIVELES)
                    {
                        al_set_sample_instance_position(recursos->musica_actual->instancia, 0); // La reinicia
                        al_play_sample_instance(recursos->musica_actual->instancia);
                    }
                   
                }
            }
        }
    }

    cambiar_tipo_municion(recursos->municiones, recursos->municion, recursos->municion_pro, *nivel_actual);
    return;
}


/**
 * @brief Función que finaliza un menú, liberando sus recursos y reseteando algunos valores.
 * @param menu El puntero al menú que se desea finalizar.
 */
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


/** 
 * @brief Función que (dibuja un rectángulo de cierto color.
 * @param rectangulo La estructura del rectángulo que se quiere dibujar (contiene posición inicial y final).
 * @param color Color del cual se desea dibujar el rectángulo.
 */
Procedure dibujar_rectangulo(Rectangulo rectangulo, ALLEGRO_COLOR color)
{
    al_draw_filled_rectangle(rectangulo.pos_inicial.x, rectangulo.pos_inicial.y, rectangulo.pos_final.x, rectangulo.pos_final.y, color);
}


/**
 * @brief Función que permite dibujar una imagen centrada en cierto porcentaje relativo a un rectángulo.
 * @param imagen La imagen que se desea colocar.
 * @param rectangulo El rectángulo sobre el cual se desea colocar la imagen.
 * @param porcentaje_x El porcentaje en x relativo al rectángulo, en el cual se desea colocar la imagen.
 * @param porcentaje_y El porcentaje en y relativo al rectángulo, en el cual se desea colocar la imagen.
 */
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


Procedure mostrar_pantalla_datos(Personaje personaje, Municion municion, Imagen vida, ALLEGRO_FONT* fuente, ALLEGRO_FONT* fuente_sec, Natural nivel_actual, float tiempo_en_nivel)
{
    Natural i, min, seg;
    Natural alto_linea, alto_barras, ancho_barras;
    Rectangulo rectangulo_subvidas;
    extern Natural puntuacion;
    char texto_nro_nivel[6] = {'\0'};
    char texto_nro_vidas[5] = {'\0'};
    char texto_puntuacion[5] = {'\0'};
    char texto_tiempo_nivel[LARGO] = {'\0'};

    alto_linea = RECTANGULO_DATOS.pos_final.y - RECTANGULO_DATOS.pos_inicial.y;

    dibujar_rectangulo(RECTANGULO_DATOS, GRIS);
    sprintf(texto_nro_nivel, "Lvl %hu", nivel_actual);
    dibujar_texto_en_rectangulo(texto_nro_nivel, RECTANGULO_DATOS, 4.0, 50.0, fuente, NEGRO);
    dibujar_rectangulo_en_rectangulo(RECTANGULO_DATOS, alto_linea, 0, 8.0, 50.0, false, NEGRO);  // Esto es equivalente a dibujar una línea (una de las dimensiones es 0)
    dibujar_imagen_en_rectangulo(vida, RECTANGULO_DATOS, 9.5, 35.0);  // Se dibuja el corazón en un 5% en x y un 50% en y del rectángulo
    sprintf(texto_nro_vidas, "× %hu", personaje.nro_vidas);
    dibujar_texto_en_rectangulo(texto_nro_vidas, RECTANGULO_DATOS, 14.5, 35.0, fuente, NEGRO);
    dibujar_texto_en_rectangulo("VIDAS", RECTANGULO_DATOS, 13.0, 75.0, fuente_sec, NEGRO);
    dibujar_rectangulo_en_rectangulo(RECTANGULO_DATOS, alto_linea, 0, 18.0, 50.0, false, NEGRO);
    rectangulo_subvidas = dibujar_rectangulo_en_rectangulo(RECTANGULO_DATOS, 40, 550, 36.5, 35.0, false, NEGRO);
    dibujar_texto_en_rectangulo("BARRA DE VIDA", RECTANGULO_DATOS, 36.5, 75.0, fuente_sec, NEGRO);

    alto_barras = 0.850 * (rectangulo_subvidas.pos_final.y - rectangulo_subvidas.pos_inicial.y);
    ancho_barras = 0.007 * (rectangulo_subvidas.pos_final.x - rectangulo_subvidas.pos_inicial.x);

    for (i=0; i<personaje.subvida_actual; i++)
    {
        dibujar_rectangulo_en_rectangulo(rectangulo_subvidas, alto_barras, ancho_barras, 0.8 + 98.4/99 * i, 50.0, true, VERDE_OSCURO);
    }

    dibujar_rectangulo_en_rectangulo(RECTANGULO_DATOS, alto_linea, 0, 55.0, 50.0, false, NEGRO);

    if (nivel_actual >= 3)
    {
        al_draw_scaled_bitmap(municion.imagen, 0, 0, personaje.balas_disponibles*al_get_bitmap_width(municion.imagen)/MAX_BALAS, al_get_bitmap_height(municion.imagen), 
                              58.0/100*ANCHO_VENTANA, ALTO_JUEGO+0.10*(ALTO_VENTANA-ALTO_JUEGO), 75.0*(float)personaje.balas_disponibles/MAX_BALAS, 50.0, 0);
    }

    else
    {
        al_draw_tinted_scaled_bitmap(municion.imagen, AMARILLO, 0, 0, al_get_bitmap_width(municion.imagen), al_get_bitmap_height(municion.imagen),
                                     58.0/100*ANCHO_VENTANA, ALTO_JUEGO+0.10*(ALTO_VENTANA-ALTO_JUEGO), 75.0, 50.0, 0);

        al_draw_line(58.0/100*ANCHO_VENTANA, ALTO_JUEGO+0.10*(ALTO_VENTANA-ALTO_JUEGO), 58.0/100*ANCHO_VENTANA+75, ALTO_JUEGO+0.10*(ALTO_VENTANA-ALTO_JUEGO)+50, ROJO, 4.0);
        al_draw_line(58.0/100*ANCHO_VENTANA, ALTO_JUEGO+0.10*(ALTO_VENTANA-ALTO_JUEGO)+50, 58.0/100*ANCHO_VENTANA+75, ALTO_JUEGO+0.10*(ALTO_VENTANA-ALTO_JUEGO), ROJO, 4.0);
    }
    
    if (fabs(floor(tiempo_en_nivel) - tiempo_en_nivel) < 1./FPS && (unsigned int) tiempo_en_nivel % 5 == 0)
    {
        if (puntuacion > 0)
        {
            puntuacion--;
        }
    }
    
    dibujar_texto_en_rectangulo("MUNICIONES", RECTANGULO_DATOS, 60.5, 75.0, fuente_sec, NEGRO);
    dibujar_rectangulo_en_rectangulo(RECTANGULO_DATOS, alto_linea, 0, 66.0, 50.0, false, NEGRO);

    dibujar_texto_en_rectangulo("ALCANCE BALAS", RECTANGULO_DATOS, 73.0, 75.0, fuente_sec, NEGRO);

    if (nivel_actual >= 3)
    {
        if (personaje.bala_maximo_alcance == true)
        {
            dibujar_texto_en_rectangulo("MÁXIMO", RECTANGULO_DATOS, 72.9, 35.0, fuente, ROJO);
            dibujar_texto_en_rectangulo("MÁXIMO", RECTANGULO_DATOS, 73.0, 35.0, fuente, ROJO);
            dibujar_texto_en_rectangulo("MÁXIMO", RECTANGULO_DATOS, 73.1, 35.0, fuente, ROJO);
        }

        else
        {
            if (nivel_actual == 3)
            {
                dibujar_texto_en_rectangulo("BÁSICO", RECTANGULO_DATOS, 73.0, 35.0, fuente, NEGRO);

            }

            else
            {
                dibujar_texto_en_rectangulo("NORMAL", RECTANGULO_DATOS, 73.0, 35.0, fuente, NEGRO);
            }
        }
    }

    dibujar_rectangulo_en_rectangulo(RECTANGULO_DATOS, alto_linea, 0, 80.0, 50.0, false, NEGRO);

    dibujar_texto_en_rectangulo("PUNTUACIÓN", RECTANGULO_DATOS, 85.0, 75.0, fuente_sec, NEGRO);
    sprintf(texto_puntuacion, "%hu", puntuacion);
    dibujar_texto_en_rectangulo(texto_puntuacion, RECTANGULO_DATOS, 85.0, 35.0, fuente, NEGRO);
    dibujar_rectangulo_en_rectangulo(RECTANGULO_DATOS, alto_linea, 0, 90.0, 50.0, false, NEGRO);
    
    min = tiempo_en_nivel / 60;
    seg = (Natural) (tiempo_en_nivel) % 60;

    if (seg >= 10)
    {
        snprintf(texto_tiempo_nivel, sizeof(texto_tiempo_nivel), "%hu:%hu", min, seg);
    }

    else
    {
        snprintf(texto_tiempo_nivel, sizeof(texto_tiempo_nivel), "%hu:0%hu", min, seg);
    }
    
    dibujar_texto_en_rectangulo(texto_tiempo_nivel, RECTANGULO_DATOS, 95.0, 35.0, fuente, NEGRO);
    dibujar_texto_en_rectangulo("TIEMPO", RECTANGULO_DATOS, 95.0, 75.0, fuente_sec, NEGRO);
}


Procedure mostrar_fondo_nivel(Imagen fondos[NRO_NIVELES], Natural nivel_actual, Natural iteracion)
{
    al_clear_to_color(NEGRO);

    if (nivel_actual == 1)  // Nivel 1 no tiene fondo
    {
        determinar_color_pantalla(iteracion);
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


/**
 * @brief Procedimiento que realiza toda la lógica que se debe cubrir al manejar un menú.
 * @param recursos Estructura que contiene todos los recursos esenciales del juego.
 * @param evento Evento auxiliar que se usa para manejar los clicks y todo eso.
 * @param etapa_actual Puntero a la etapa actual (se modificará la etapa si se redirige el menú).
 * @param nivel_actual Puntero al nivel actual del juego (el nivel podría cambiar en algunos casos).
 * @param iteracion Iteración actual del juego.
 * @param cambio_estado_procesado Es un puntero que se usa al pasar ese parámetro a la función dibujar mapa.
 * @param ultima_tecla_lateral Puntero a la última tecla lateral (se usa para dibujar a Woofson).
 * @param ranking_a_mostrar Puntero al ranking que se está mostrando actualmente.
 * @param tiempo_en_nivel Tiempo actual en el nivel (puntero -> puede cambiar).
 */
Procedure manejar_menu(Recursos* recursos, ALLEGRO_EVENT* evento, Etapa* etapa_actual, Natural* nivel_actual, Natural iteracion, bool* cambio_estado_procesado, Tecla* ultima_tecla_lateral, Natural* ranking_a_mostrar, float* tiempo_en_nivel)
{
    Menu menu_vacio = {0};

    if (evento->type == CLICK)
    {
        recursos->menu_actual.opcion_en_hover = obtener_opcion_en_hover(recursos->menu_actual);

        if (recursos->menu_actual.opcion_en_hover < recursos->menu_actual.nro_opciones)
        {
            redirigir_menu(recursos, recursos->menu_actual.opcion_en_hover, etapa_actual, nivel_actual, ranking_a_mostrar, tiempo_en_nivel);
        }
    }

    else if (evento->type == ALLEGRO_EVENT_TIMER)
    {
        if (memcmp(&recursos->menu_actual, &menu_vacio, sizeof(Menu)) == 0)
        {
            recursos->menu_actual.opcion_en_hover = -1;

            if (*etapa_actual == VICTORIA || *etapa_actual == DERROTA)
            {
                redirigir_menu(recursos, recursos->menu_actual.opcion_en_hover, etapa_actual, nivel_actual, ranking_a_mostrar, tiempo_en_nivel);
            }
        }

        if (*nivel_actual != 0)
        {            
            mostrar_fondo_nivel(recursos->fondos, *nivel_actual, iteracion);
            dibujar_mapa(recursos->mapas[(*nivel_actual)-1], recursos, cambio_estado_procesado, iteracion);  // Dibujamos el mapa del primer nivel
            dibujar_personaje(&recursos->pje_principal, *ultima_tecla_lateral, iteracion);  // Dibuja el personaje en su posición actual
            morir(&recursos->pje_principal, ultima_tecla_lateral, etapa_actual);  // Esta función se ejecuta solamente si el personaje figura como muerto
            actualizar_rayos(recursos->rayos[(*nivel_actual)-1], recursos->cantidad_rayos[(*nivel_actual)-1], recursos->pje_principal, recursos->mapas[(*nivel_actual)-1]);
            mostrar_pantalla_datos(recursos->pje_principal, recursos->municiones[0], recursos->vida, recursos->fuentes[COMFORTAA_LIGHT_GIGANTE], recursos->fuentes[TIMES_NEW_ROMAN_NORMAL], *nivel_actual, *tiempo_en_nivel);
        }
    
        if (*etapa_actual != RANKING)
        {
            mostrar_menu(recursos->menu_actual, *etapa_actual);
        }

        else
        {
            mostrar_ranking(&recursos->menu_actual, &recursos->rankings[*ranking_a_mostrar]);
        }
    }
}