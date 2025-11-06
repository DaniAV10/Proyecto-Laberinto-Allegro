// celda.h
#pragma once
#include <vector>

//Representa una celda del laberinto
//Cada celda puede tener muros en sus cuatro lados y un estado de visita.
//Esta estructura es utilizada por el generador de laberintos y por el visualizador/movimiento del jugador.
struct Celda {
    bool muroNorte = true;   // ¿Tiene muro arriba?
    bool muroSur = true;     // ¿Tiene muro abajo?
    bool muroEste = true;    // ¿Tiene muro a la derecha?
    bool muroOeste = true;   // ¿Tiene muro a la izquierda?
    bool visitada = false;   // ¿Ya fue visitada por el algoritmo?
};
