// generador.h
#pragma once
#include <vector>
#include "celda.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

void generarLaberinto(std::vector<std::vector<Celda>>& laberinto); // Recibe la matriz del laberinto y modifica sus muros para crear un camino válido



