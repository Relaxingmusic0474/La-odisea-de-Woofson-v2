#ifndef RANK_H
#define RANK_H

#include "allegro_libraries.h"
#include "stdio.h"
#include "types.h"
#include "structs.h"
#include "score.h"

bool leer_ranking(Ranking*, Natural);  // Lee el ranking de un cierto nivel
bool modificar_ranking(Ranking*, Natural);  // Modifica el ranking de un cierto nivel
Procedure insertar_en_ranking(Ranking*, char*, Natural);


#endif // RANK_H