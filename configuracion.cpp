// configuracion.cpp
#include "configuracion.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

void mostrarConfiguracion(ALLEGRO_FONT* fuente, int ancho, int alto) {
    al_clear_to_color(al_map_rgb(0, 0, 0)); // Fondo negro

    // Título
    al_draw_text(fuente, al_map_rgb(0, 255, 0), 100, 50, 0, "Configuracion del laberinto");

    // Texto de ancho
    char textoAncho[100];
    sprintf_s(textoAncho, sizeof(textoAncho), "Ancho: %d   (Izquierda / Derecha)", ancho);
    al_draw_text(fuente, al_map_rgb(255, 255, 255), 100, 120, 0, textoAncho);

    // Texto de alto
    char textoAlto[100];
    sprintf_s(textoAlto, sizeof(textoAlto), "Alto:  %d   (Arriba / Abajo)", alto);
    al_draw_text(fuente, al_map_rgb(255, 255, 255), 100, 160, 0, textoAlto);

    // Instrucción para continuar
    al_draw_text(fuente, al_map_rgb(255, 255, 0), 100, 220, 0,  "Presiona ENTER para continuar");

    al_flip_display(); // Muestra en pantalla
}

bool manejarConfiguracion(ALLEGRO_EVENT& evento, int& ancho, int& alto) {
    if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (evento.keyboard.keycode) {
        case ALLEGRO_KEY_LEFT:
            if (ancho > 5) ancho--;
            break;
        case ALLEGRO_KEY_RIGHT:
            if (ancho < 50) ancho++;
            break;
        case ALLEGRO_KEY_UP:
            if (alto > 5) alto--;
            break;
        case ALLEGRO_KEY_DOWN:
            if (alto < 50) alto++;
            break;
        case ALLEGRO_KEY_ENTER:
            return true; // Confirmar configuración
        }
    }
    return false; // Aún no confirmado
}
