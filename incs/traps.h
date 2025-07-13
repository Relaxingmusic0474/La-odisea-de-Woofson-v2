#ifndef TRAPS_H
#define TRAPS_H

#include "structs.h"

// Funciones para manejar trampas
Procedure inicializar_rayo(Rayo* rayo);
Procedure activar_rayo(Rayo* rayo);
Procedure desactivar_rayo(Rayo* rayo);
bool verificar_colision_rayo(Personaje* personaje, Rayo* rayo);

#endif // TRAPS_H