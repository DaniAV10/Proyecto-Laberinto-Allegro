// menu.cpp
#include "menu.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

//Variable global para saber qué opción está seleccionada
//Se actualiza con las flechas arriba/abajo.
int opcionSeleccionada = 0;

//Dibuja el menú principal en pantalla.
//Recibe la fuente para mostrar el texto.
//Resalta la opción seleccionada en amarillo.
void mostrarMenu(ALLEGRO_FONT* fuente) {
    //Limpia la pantalla y la pinta de negro
    al_clear_to_color(al_map_rgb(0, 0, 0));

    // Carga el icono desde la carpeta del proyecto
    ALLEGRO_BITMAP* icono = al_load_bitmap("iconos/icon.png");
    int anchoPantalla = al_get_display_width(al_get_current_display());

    // Dibuja el icono centrado arriba (si se cargó correctamente)
    if (icono) {
        int iconoAncho = al_get_bitmap_width(icono);
        int iconoAlto = al_get_bitmap_height(icono);
        al_draw_bitmap(icono, anchoPantalla / 2 - iconoAncho / 2, 20, 0);
    }

    // Lista de opciones del menú
    const char* opciones[] = {
        "1. Generar laberinto y jugar",
        "2. Estadisticas globales",
        "3. Ver resumen global",
        "4. Salir"
    };

    //Dibuja el título del menú en verde
    al_draw_text(fuente, al_map_rgb(0, 255, 0), anchoPantalla / 2, 100, ALLEGRO_ALIGN_CENTER, "Menu Principal-Laberinto Allegro");

    //Recorre las opciones y las dibuja en pantalla
    for (int i = 0; i < 4; i++) {
        //Si la opción está seleccionada, se dibuja en amarillo. Si no, la dibuja en blanco
        ALLEGRO_COLOR color = (i == opcionSeleccionada) ? al_map_rgb(255, 255, 0) : al_map_rgb(255, 255, 255);
        //Dibuja la opción en la posición correspondiente (centrada y con más espacio vertical)
        al_draw_text(fuente, color, anchoPantalla / 2, 160 + i * 50, ALLEGRO_ALIGN_CENTER, opciones[i]);
    }

    //Dibuja los nombres del equipo en rojo más abajo
    al_draw_text(fuente, al_map_rgb(255, 0, 0), anchoPantalla / 2, 400, ALLEGRO_ALIGN_CENTER, "-Daniel Alejandro Arrieta Viquez-");
    al_draw_text(fuente, al_map_rgb(255, 0, 0), anchoPantalla / 2, 440, ALLEGRO_ALIGN_CENTER, "-Ronny Espinoza Corderor-");
    al_draw_text(fuente, al_map_rgb(255, 0, 0), anchoPantalla / 2, 480, ALLEGRO_ALIGN_CENTER, "-Cristhofer Herrera-");

    //Actualiza la pantalla para mostrar los cambios
    al_flip_display();

    // Libera el icono después de dibujarlo
    if (icono) {
        al_destroy_bitmap(icono);
    }
}


//Maneja los eventos de teclado para navegar por el menú.
//Flechas arriba/abajo cambian la opción seleccionada.
//ENTER confirma la selección y devuelve el estado correspondiente.
//Si no se presiona ENTER, se mantiene en MENU_PRINCIPAL.
EstadoPrograma manejarMenu(ALLEGRO_EVENT& evento) {
    if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (evento.keyboard.keycode) {
        case ALLEGRO_KEY_UP:
            opcionSeleccionada = (opcionSeleccionada - 1 + 4) % 4; // Subir opción correctamente
            break;
        case ALLEGRO_KEY_DOWN:
            opcionSeleccionada = (opcionSeleccionada + 1) % 4; // Bajar opción
            break;
        case ALLEGRO_KEY_ENTER:
            switch (opcionSeleccionada) {
            case 0: return CONFIGURACION_LABERINTO;
            case 1: return ESTADISTICAS;
            case 2: return RESUMEN_GLOBAL;
            case 3: return SALIR;
            }
        }
    }
    return MENU_PRINCIPAL; //Si no se presiona ENTER, se mantiene en el menú principal
}