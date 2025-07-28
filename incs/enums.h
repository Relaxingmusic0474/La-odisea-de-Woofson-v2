#ifndef ENUMS_H
#define ENUMS_H

enum tipoFrame
{
    FRAME_WOOFSON,
    FRAME_DRAGON,
    FRAME_EXTRATERRESTRE,
    FRAME_MONSTRUO,
    TIPOS_PERSONAJES,
};

typedef enum tipoFrame TipoFrame;

enum tipoPersonaje
{
    WOOFSON = 'W',
    DRAGON = 'D',
    EXTRATERRESTRE = 'E',
    MONSTRUO = 'M',
};

typedef enum tipoPersonaje TipoPersonaje;

enum fuente
{
    COMFORTAA_LIGHT,
    TIMES_NEW_ROMAN,
};

typedef enum fuente Fuente;

enum tipoFuente
{
    COMFORTAA_LIGHT_NORMAL,
    TIMES_NEW_ROMAN_NORMAL,
    COMFORTAA_LIGHT_GRANDE,
    TIMES_NEW_ROMAN_GRANDE,
    COMFORTAA_LIGHT_GIGANTE,
    TIMES_NEW_ROMAN_GIGANTE,
};

typedef enum tipoFuente TipoFuente;

enum etapa
{
    MENU_PRINCIPAL = -4,  // Se partió en -4 estratégicamente para que el índice del nivel 1 sea 0, y se pueda usar el mismo índice para acceder al mapa
    MENU_NIVELES,
    INSTRUCCIONES,
    RANKING,
    NIVEL1,
    NIVEL2,
    NIVEL3,
    NIVEL4,
    NIVEL5,
    DERROTA,
    VICTORIA,
    MENU_RANKING,
};

typedef enum etapa Etapa;

enum tipoAudio
{
    MUSICA_MENU,
    MUSICA_NIVEL_1,
    MUSICA_NIVEL_2,
    MUSICA_NIVEL_3,
    MUSICA_NIVEL_4,
    MUSICA_NIVEL_5, 
};

typedef enum tipoAudio TipoAudio;

enum tipoEfecto
{
    SONIDO_RAYO,
};

typedef enum tipoEfecto TipoEfecto;

enum etapaRayo
{
    INOPERATIVO,  // Etapa en la cual el rayo no está activo (no está funcionando)
    EN_ESPERA,  // Etapa de espera del rayo (cuando el rayo está inactivo pero sí está funcionando y listo para el ataque continuo)
    EN_APARICION,  // Etapa de activación del rayo
    ACTIVO_AL_100,  // Etapa en la cual el rayo está activo al 100% de su longitud
    EN_DESAPARICION,  // Etapa de desaparición del rayo (cuando de nuevo se volverá a poner en espera)
};

typedef enum etapaRayo EtapaRayo;

enum modoWoofson
{
    CAMINATA = 'M',  // M de movimiento (normal)
    PELEA = 'P',  // P de pelea
    DISPARO = 'D',  // D de disparo
};

typedef enum modoWoofson ModoWoofson;

enum estadoPuerta
{
    CERRADA,
    ABIERTA,
};

typedef enum estadoPuerta EstadoPuerta;

enum estadoPalanca
{
    DESACTIVADA,
    ACTIVADA,
};

typedef enum estadoPalanca EstadoPalanca;

enum tipoMenu
{
    PRINCIPAL,
    NIVELES,
    PERDER,
    GANAR,
};

typedef enum tipoMenu TipoMenu;

#endif  // ENUMS_H