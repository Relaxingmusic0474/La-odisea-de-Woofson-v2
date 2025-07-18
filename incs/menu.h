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

Vector obtener_posicion_mouse(void); 
Natural obtener_opcion_en_hover(Menu);
Procedure determinar_color_pantalla(Natural);
bool inicializar_menu_principal(Menu*, ALLEGRO_FONT*);
bool inicializar_menu_niveles(Menu*, ALLEGRO_FONT*); 
Procedure mostrar_menu(Menu);
Procedure redirigir_menu(Recursos*, ALLEGRO_FONT*, Natural, Etapa*, Natural*);
Procedure finalizar_menu(Menu*);
Procedure dibujar_rectangulo(Rectangulo rectangulo, ALLEGRO_COLOR color);
Procedure dibujar_imagen_en_rectangulo(Imagen, Rectangulo, float, float);
Procedure dibujar_texto_en_rectangulo(char*, Rectangulo, float, float, ALLEGRO_FONT*);
Rectangulo dibujar_rectangulo_en_rectangulo(Rectangulo, float, float, float, float, bool, ALLEGRO_COLOR);
Procedure mostrar_pantalla_datos(Personaje, ALLEGRO_BITMAP*, ALLEGRO_FONT*, ALLEGRO_FONT*, Natural);
Procedure mostrar_fondo_nivel(Imagen [NRO_NIVELES], Natural, Natural);

#endif  // MENU_H