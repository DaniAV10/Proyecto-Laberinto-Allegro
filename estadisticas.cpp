// estadisticas.cpp
#include "estadisticas.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <map>
#include <sstream>
#include <algorithm>
#include <queue>
#include <tuple>
#include "celda.h"


//Guarda los datos de una partida en un archivo de texto
void guardarEstadistica(int ancho, int alto, int movimientos, double tiempoSegundos, int caminoOptimo) {
    std::ofstream archivo("maze_solver_log.txt", std::ios::app);
    if (archivo.is_open()) {
        archivo << "Size: " << ancho << "x" << alto
            << ", Movimientos: " << movimientos
            << ", Tiempo: " << std::fixed << std::setprecision(2) << tiempoSegundos << "s"
            << ", CaminoOptimo: " << caminoOptimo << "\n";
        archivo.close();
    }
}

//Muestra las estadísticas guardadas en consola
void mostrarEstadisticasGlobales() {
    std::ifstream archivo("maze_solver_log.txt");
    if (archivo.is_open()) {
        std::string linea;
        std::cout << "\n=== Estadísticas de partidas anteriores ===\n";
        while (std::getline(archivo, linea)) {
            std::cout << linea << "\n";
        }
        std::cout << "===========================================\n\n";
        archivo.close();
    }
    else {
        std::cout << "No se encontraron estadísticas.\n";
    }
}

void mostrarResumenEstadisticasGlobales(ALLEGRO_FONT* fuente) {
    std::ifstream archivo("maze_solver_log.txt");
    std::map<std::string, double> mejorTiempo;
    std::map<std::string, double> sumaEficiencia;
    std::map<std::string, int> cantidad;

    std::vector<std::string> resumenLineas;

    if (archivo.is_open()) {
        std::string linea;
        while (std::getline(archivo, linea)) {
            size_t posSize = linea.find("Size: ");
            size_t posMov = linea.find("Movimientos: ");
            size_t posTiempo = linea.find("Tiempo: ");

            if (posSize != std::string::npos && posMov != std::string::npos && posTiempo != std::string::npos) {
                std::string dimension = linea.substr(posSize + 6, posMov - posSize - 7);
                std::string movStr = linea.substr(posMov + 12, posTiempo - posMov - 13);
                std::string tiempoStr = linea.substr(posTiempo + 8);

                if (!tiempoStr.empty() && tiempoStr.back() == 's') {
                    tiempoStr.pop_back();
                }

                try {
                    int movimientos = std::stoi(movStr);
                    double tiempo = std::stod(tiempoStr);

                    if (mejorTiempo.find(dimension) == mejorTiempo.end() || tiempo < mejorTiempo[dimension]) {
                        mejorTiempo[dimension] = tiempo;
                    }

                    int ancho = std::stoi(dimension.substr(0, dimension.find('x')));
                    int alto = std::stoi(dimension.substr(dimension.find('x') + 1));
                    size_t posOptimo = linea.find("CaminoOptimo: ");
                    int caminoOptimo = (posOptimo != std::string::npos)
                        ? std::stoi(linea.substr(posOptimo + 14))
                        : ancho * alto;

                    double eficiencia = static_cast<double>(caminoOptimo) / movimientos;
                    sumaEficiencia[dimension] += eficiencia;
                    cantidad[dimension]++;
                }
                catch (...) {}
            }
        }
        archivo.close();
    }


    // Construir líneas de resumen
    for (const auto& par : mejorTiempo) {
        std::string dimension = par.first;
        double mejor = par.second;
        double eficienciaProm = sumaEficiencia[dimension] / cantidad[dimension];

        std::string texto1 = "Mejor tiempo para " + dimension + ": " + std::to_string(mejor) + "s";
        std::string texto2 = "Eficiencia promedio: " + std::to_string(eficienciaProm);

        resumenLineas.push_back(texto1);
        resumenLineas.push_back(texto2);
        resumenLineas.push_back(""); // línea vacía para separación
    }

    // Parámetros visuales
    const int salto = 24;
    const int margenX = 50;
    const int margenY = 80;
    const int altoVentana = 600;
    const int maxVisible = (altoVentana - margenY - 50) / salto;

    int desplazamiento = 0;

    // Cola de eventos
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_EVENT evento;
    al_register_event_source(queue, al_get_keyboard_event_source());

    bool esperando = true;
    while (esperando) {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_text(fuente, al_map_rgb(0, 255, 0), margenX, 30, 0, "Resumen de Estadisticas Globales:");
        al_draw_text(fuente, al_map_rgb(255, 255, 255), margenX, altoVentana - 30, 0,
            "Usa (Arriba / Abajo) para desplazarte, ESC para salir");

        for (size_t i = desplazamiento; i < resumenLineas.size() && i < desplazamiento + maxVisible; ++i) {
            al_draw_text(fuente, al_map_rgb(255, 255, 255),
                margenX, margenY + (i - desplazamiento) * salto, 0, resumenLineas[i].c_str());
        }

        al_flip_display();

        al_wait_for_event(queue, &evento);
        if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                esperando = false;
            }
            else if (evento.keyboard.keycode == ALLEGRO_KEY_UP && desplazamiento > 0) {
                desplazamiento--;
            }
            else if (evento.keyboard.keycode == ALLEGRO_KEY_DOWN &&
                desplazamiento + maxVisible < resumenLineas.size()) {
                desplazamiento++;
            }
        }
    }

    al_destroy_event_queue(queue);
}

//Muestra las estadísticas en pantalla
void mostrarEstadisticasEnPantalla(ALLEGRO_FONT* fuente) {
    std::ifstream archivo("maze_solver_log.txt");
    std::vector<std::string> lineas;

    if (archivo.is_open()) {
        std::string linea;
        while (std::getline(archivo, linea)) {
            lineas.push_back(linea);
        }
        archivo.close();
    }
    else {
        lineas.push_back("No se encontraron estadisticas.");
    }

    // Parámetros visuales
    const int salto = 24; // Espacio entre líneas
    const int margenX = 50;
    const int margenY = 80;
    const int altoVentana = 600; // Ajustalo si tu ventana tiene otro alto
    const int maxVisible = (altoVentana - margenY - 50) / salto; // Líneas visibles

    int desplazamiento = 0;

    // Cola de eventos
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_EVENT evento;
    al_register_event_source(queue, al_get_keyboard_event_source());

    bool esperando = true;
    while (esperando) {
        // Dibujar fondo y título
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_text(fuente, al_map_rgb(0, 255, 0), margenX, 30, 0,
            "Estadisticas de partidas anteriores:");
        al_draw_text(fuente, al_map_rgb(255, 255, 255), margenX, altoVentana - 30, 0,
            "Usa (Arriba / Abajo) para desplazarte, ESC para salir");

        // Dibujar líneas visibles
        for (size_t i = desplazamiento; i < lineas.size() && i < desplazamiento + maxVisible; ++i) {
            al_draw_text(fuente, al_map_rgb(255, 255, 255),
                margenX, margenY + (i - desplazamiento) * salto, 0, lineas[i].c_str());
        }

        al_flip_display();

        // Esperar evento
        al_wait_for_event(queue, &evento);
        if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                esperando = false;
            }
            else if (evento.keyboard.keycode == ALLEGRO_KEY_UP && desplazamiento > 0) {
                desplazamiento--;
            }
            else if (evento.keyboard.keycode == ALLEGRO_KEY_DOWN &&
                desplazamiento + maxVisible < lineas.size()) {
                desplazamiento++;
            }
        }
    }

    al_destroy_event_queue(queue);
}

int calcularCaminoOptimoBFS(const std::vector<std::vector<Celda>>& laberinto, int inicioX, int inicioY, int metaX, int metaY) {
    int alto = laberinto.size();
    int ancho = laberinto[0].size();

    std::vector<std::vector<bool>> visitado(alto, std::vector<bool>(ancho, false));
    std::queue<std::tuple<int, int, int>> cola; // x, y, pasos

    cola.push({ inicioX, inicioY, 0 });
    visitado[inicioY][inicioX] = true;

    int dx[] = { 0, 0, -1, 1 };
    int dy[] = { -1, 1, 0, 0 };

    while (!cola.empty()) {
        std::tuple<int, int, int> actual = cola.front();
        int x = std::get<0>(actual);
        int y = std::get<1>(actual);
        int pasos = std::get<2>(actual);
        cola.pop();

        if (x == metaX && y == metaY) {
            return pasos;
        }

        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (nx >= 0 && ny >= 0 && nx < ancho && ny < alto && !visitado[ny][nx]) {
                const Celda& actual = laberinto[y][x];
                const Celda& vecino = laberinto[ny][nx];

                bool conectado = false;
                if (i == 0 && !actual.muroNorte && !vecino.muroSur) conectado = true;
                if (i == 1 && !actual.muroSur && !vecino.muroNorte) conectado = true;
                if (i == 2 && !actual.muroOeste && !vecino.muroEste) conectado = true;
                if (i == 3 && !actual.muroEste && !vecino.muroOeste) conectado = true;

                if (conectado) {
                    visitado[ny][nx] = true;
                    cola.push(std::make_tuple(nx, ny, pasos + 1));
                }
            }
        }
    }

    return -1; // No se encontró camino
}