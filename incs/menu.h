#ifndef MENU_H 
#define MENU_H 

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "structs.h"
#include "enums.h" 
#include "types.h"
#include "macros.h"
#include "load.h"
#include "score.h"
#include "rank.h"
#include "allegro_libraries.h"

Vector obtener_posicion_mouse(void); 
Natural obtener_opcion_en_hover(Menu);
Procedure determinar_color_pantalla(Natural);
bool inicializar_menu_principal(Menu*, ALLEGRO_FONT*);
bool inicializar_menu_niveles(Menu*, ALLEGRO_FONT*); 
bool inicializar_menu(Menu*, TipoMenu, Imagen, ALLEGRO_FONT*, ALLEGRO_FONT*, char* [], Natural, Rectangulo);
Procedure mostrar_menu(Menu, Etapa);
Procedure mostrar_ranking(Menu*, Ranking*);//, ALLEGRO_FONT*);
Procedure cambiar_menu(Menu*, Menu);
Procedure cambiar_tipo_municion(Municion [MAX_MUNICIONES], Imagen, Imagen, Natural);
Procedure resetear_estado_juego(Recursos*, Menu, Etapa*, Etapa, float*);
Procedure detener_efectos_de_sonido(Recursos*);
Procedure redirigir_menu(Recursos*, /*ALLEGRO_FONT*, */Natural, Etapa*, Natural*, Natural*, float*);
Procedure finalizar_menu(Menu*);
Procedure dibujar_rectangulo(Rectangulo rectangulo, ALLEGRO_COLOR color);
Procedure dibujar_imagen_en_rectangulo(Imagen, Rectangulo, float, float);
Procedure dibujar_texto_en_rectangulo(char*, Rectangulo, float, float, ALLEGRO_FONT*, ALLEGRO_COLOR);
Rectangulo obtener_rectangulo(Rectangulo, float, float, float, float);
Rectangulo dibujar_rectangulo_en_rectangulo(Rectangulo, float, float, float, float, bool, ALLEGRO_COLOR);
Procedure mostrar_pantalla_datos(Personaje, Municion, Imagen, ALLEGRO_FONT*, ALLEGRO_FONT*, Natural, float);
Procedure mostrar_fondo_nivel(Imagen [NRO_NIVELES], Natural, Natural);
Procedure manejar_menu(Recursos*, ALLEGRO_EVENT*, Etapa*, Natural*, Natural, bool*, Tecla*, Natural*, float*);

#endif  // MENU_H