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
Procedure redirigir_menu(Menu*, ALLEGRO_FONT*, Natural, Etapa*);
Procedure finalizar_menu(Menu*);
Procedure mostrar_pantalla_datos(Personaje, ALLEGRO_BITMAP*);

#endif  // MENU_H