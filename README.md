# Juego en C

### Función de cada uno de los archivos:

#### Archivos de código fuente:

1. woofson.c: Contiene todo el flujo principal del programa.
2. allegro.c: Contiene todo lo que tiene que ver con la inicialización y finalización de allegro, además de la carga o inicialización de otros elementos esenciales.
3. menu.c: Contiene las definiciones de las funciones que tienen que ver con el menú, y también con la visualización en general.
4. load.c: Contiene las definiciones de las funciones que sirven para cargar algunas cosas como el mapa, la música, etc.
5. characters.c: Contiene las definiciones de las funciones que tienen que ver con el personaje, su movimiento y su forma de dibujarlo.
6. collisions.c: Contiene las definiciones de las funciones que manejan las colisiones del personaje al moverse.
7. debug.c: Contiene la implementación de funciones que pueden servir para depurar el programa en caso de errores. 

#### Archivos de cabecera (headers):

1. allegro_libraries.h: Incluye las librerías de allegro5.
2. allegro.h: Contiene los prototipos de las funciones que se encargan del manejo esencial de Allegro y otras cosas esenciales, definidas en allegro.c.
3. menu.h: Contiene los prototipos de las funciones definidas en menu.c (encargadas del menú, y en general de la visualización).
4. load.h: COntiene los prototipos de las funciones definidas en load.c (encargadas de la carga de algunas cosas como el mapa y la música).
5. characters.h: Contiene los prototipos de las funciones definidas en characters.c (relacionadas con el personaje).
6. collisions.h: Contiene los prototipos de las funciones definidas en collisions.c (relacionadas con las colisiones y su manejo).
7. debug.h: Contiene los prototipos de las funciones de depuración, definidas en debug.c.
8. macros.h: Contiene todas las macros que se utilizarán a nivel global del programa.
9. structs.h: Contiene la mayoría de las estructuras de datos que se implementarán en el juego (Vector, Mapa, Recursos, Personaje, etc).
10. enums.h: Contiene la mayoría de las enumeraciones de datos que serán útiles en el juego para mayor legibilidad.
11. types.h: Contiene redefiniciones de algunos tipos de datos según mi estilo de programación.
