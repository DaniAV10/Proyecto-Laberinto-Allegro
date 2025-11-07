//victoria.cpp

#include "victoria.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

void mostrarPantallaVictoria(ALLEGRO_FONT* fuente, DatosPartida datos) {
	//Fondo de pantalla
	al_clear_to_color(al_map_rgb(0, 50, 0)); //Color verde obscuro

	//Titulo de pantalla
	al_draw_text(fuente, al_map_rgb(255, 215, 0), 400, 80, ALLEGRO_ALIGN_CENTER, "VICTORIA");

	//Mostrar estadisticas
	al_draw_textf(fuente, al_map_rgb(255, 255, 255), 400, 150, ALLEGRO_ALIGN_CENTER, "Tamano del laberinto: %d x %d", datos.anchoLaberinto, datos.altoLaberinto);
	al_draw_textf(fuente, al_map_rgb(255, 255, 255), 400, 180, ALLEGRO_ALIGN_CENTER, "Movimientos: %d", datos.movimientos);
	al_draw_textf(fuente, al_map_rgb(255, 255, 255), 400, 210, ALLEGRO_ALIGN_CENTER, "Tiempo: %.1f segundos", datos.tiempo);

	//Opciones de pantalla
	al_draw_text(fuente, al_map_rgb(200, 200, 255), 400, 280, ALLEGRO_ALIGN_CENTER, "1. Jugar otro laberinto");
	al_draw_text(fuente, al_map_rgb(200, 200, 255), 400, 310, ALLEGRO_ALIGN_CENTER, "2. Volver al menu principal");
	al_draw_text(fuente, al_map_rgb(200, 200, 255), 400, 340, ALLEGRO_ALIGN_CENTER, "ESC. Salir del juego");
}

int manejarVictoria(ALLEGRO_EVENT evento) {
	if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
		switch (evento.keyboard.keycode) {
		case ALLEGRO_KEY_1:
			return 1; //Jugar otro laberinto
		case ALLEGRO_KEY_2:
			return 2; //Volver al menu principal
		case ALLEGRO_KEY_ESCAPE:
			return 3; //Salir
		}
	}

	return 0; //En otro caso
}