#ifndef ENUMS_H
#define ENUMS_H

enum fuente
{
    COMFORTAA_LIGHT,
    TIMES_NEW_ROMAN,
};

typedef enum fuente Fuente;

enum etapa
{
    MENU_PRINCIPAL,
    MENU_NIVELES,
    INSTRUCCIONES,
    RANKING,
    NIVEL1,
    NIVEL2,
    NIVEL3,
    NIVEL4,
    NIVEL5,
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

enum tipoColision
{
    INEXISTENTE,
    IZQUIERDA,
    DERECHA,
    SUPERIOR,
    INFERIOR,
};

typedef enum tipoColision TipoColision;

#endif  // ENUMS_H