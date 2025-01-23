#ifndef MENU_H 
#define MENU_H 

#include "stdio.h"
#include "structs.h"
#include "enums.h" 
#include "types.h"
#include "macros.h"
#include "load.h"

Posicion obtener_posicion_mouse(void); 
Natural obtener_opcion_en_hover(Menu); 
bool inicializar_menu(Menu*); 
Procedure mostrar_menu(Menu); 

#endif