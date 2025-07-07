#ifndef ALLEGRO_LIBRARIES_H
#define ALLEGRO_LIBRARIES_H

#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro_audio.h"
#include "allegro5/allegro_acodec.h"
#include "allegro5/allegro_image.h"

#define TECLA_PRESIONADA (ALLEGRO_EVENT_KEY_DOWN)
#define TECLA_LIBERADA (ALLEGRO_EVENT_KEY_UP)
#define CLICK (ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
#define RATON_LIBRE (ALLEGRO_EVENT_MOUSE_BUTTON_UP)
#define RATON_MOVIENDOSE (ALLEGRO_EVENT_MOUSE_AXES)

typedef ALLEGRO_BITMAP* Imagen;
typedef ALLEGRO_SAMPLE* Audio;
typedef ALLEGRO_SAMPLE_INSTANCE* InstanciaAudio;
typedef ALLEGRO_DISPLAY* Ventana;
typedef int Tecla;

#endif  // ALLEGRO_LIBRARIES_H