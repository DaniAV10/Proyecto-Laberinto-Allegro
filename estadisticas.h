// estadisticas.h
#pragma once
#include <string>
#include <allegro5/allegro_font.h>  
#include <vector>
#include "celda.h"

//Guarda una partida en el archivo de estadísticas
//Se debe llamar al finalizar una partida
void guardarEstadistica(int ancho, int alto, int movimientos, double tiempoSegundos, int caminoOptimo);

//Muestra las estadísticas en consola
void mostrarEstadisticasGlobales();

//Muestra las estadísticas en pantalla usando Allegro
void mostrarEstadisticasEnPantalla(ALLEGRO_FONT* fuente);

void mostrarResumenEstadisticasGlobales(ALLEGRO_FONT* fuente);

int calcularCaminoOptimoBFS(const std::vector<std::vector<Celda>>& laberinto, int inicioX, int inicioY, int metaX, int metaY);