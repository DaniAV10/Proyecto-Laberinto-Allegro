// juego.cpp
#include "juego.h"
#include "laberinto.h"
#include "estadisticas.h"
#include <iostream>
#include "generador.h"
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
//Esta funcion será reemplazada por (Persona 3/Cristhofer)-Apunte de Daniel
//Se deben agregar las partes del juego: mover jugador, detectar meta y un par de cosas más.
/* El renderizado es solo modificar un poco lo que ya se tiene en la funcion ejecutarJuego.
Y agregar aspectos simples de formato.*/
//En este momento, puse temporalmente que simule una partida para probar el sistema de estadisticas
DatosPartida ejecutarJuego(ALLEGRO_FONT* fuente, ALLEGRO_DISPLAY* displayPrincipal) {
    std::cout << "Juego iniciado con laberinto de " << anchoLaberinto << "x" << altoLaberinto << "\n";

    std::vector<std::vector<Celda>> laberinto(altoLaberinto, std::vector<Celda>(anchoLaberinto));
    generarLaberinto(laberinto);
    
    // Márgenes visuales
    int margenX = 150;
    int margenY = 50;

    // Tamaño fijo de ventana
    const int ventanaAncho = 800;
    const int ventanaAlto = 600;

    // Calcular tamaño de celda para que el laberinto quepa en la ventana
    int tamCelda = std::min(
        (ventanaAncho - margenX * 2) / anchoLaberinto,
        (ventanaAlto - margenY * 2) / altoLaberinto
    );

    
    // Posición inicial del jugador (centro de la primera celda)
    float jugadorX = margenX + tamCelda / 2.0f;
    float jugadorY = margenY + tamCelda / 2.0f;

    // Tamaño del jugador y meta escalados
    float radioJugador = tamCelda / 6.0f;
    float velocidad = tamCelda / 6.0f;

    // Posición de la meta (centro de la última celda)
    int metaX = anchoLaberinto - 1;
    int metaY = altoLaberinto - 1;
    float metaPosX = margenX + metaX * tamCelda + tamCelda / 2.0f;
    float metaPosY = margenY + metaY * tamCelda + tamCelda / 2.0f;
    float tamMeta = tamCelda / 2.0f;

    bool juegoGanado = false;
    bool teclas[ALLEGRO_KEY_MAX] = { false };
    ALLEGRO_DISPLAY* display = al_get_current_display();
    ALLEGRO_DISPLAY* displayJuego = displayPrincipal;

    //Contadores para obtener estadisticas
    int movimientos = 0;
    int movimientosframe = 0;
    double tiempoInicio = al_get_time();
    double tiempoSegundos = 0.0;

    // Sonido de victoria
     ALLEGRO_SAMPLE* sonidoVictoria = NULL;
     bool sonidoReproducido = false;

     // Inicializar audio (agregar esto antes de cargar sonidos)
     if (!al_install_audio()) {
         std::cout << "Error inicializando audio\n";
     }
     if (!al_init_acodec_addon()) {
         std::cout << "Error inicializando codecs de audio\n";
     }
     if (!al_reserve_samples(1)) {
         std::cout << "Error reservando samples de audio\n";
     }

     // Cargar sonido de victoria
     sonidoVictoria = al_load_sample("sonidos/victoria.wav");
     if (!sonidoVictoria) {
         std::cout << "No se pudo cargar el sonido de victoria\n";
     }

	//Cola de eventos y bucle principal
   
	// Bucle principal del juego
    bool redibujar = true;
    bool salirPorVictoria = false;

    ALLEGRO_EVENT_QUEUE* colaEventos = al_create_event_queue();
    al_register_event_source(colaEventos, al_get_display_event_source(display));
    al_register_event_source(colaEventos, al_get_keyboard_event_source());

    while (redibujar) {//Ciclo para generar.
        ALLEGRO_EVENT evento;
        al_wait_for_event(colaEventos, &evento);//

        if (!juegoGanado) {
            if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
                teclas[evento.keyboard.keycode] = true;
            else if (evento.type == ALLEGRO_EVENT_KEY_UP)
                teclas[evento.keyboard.keycode] = false;
        }

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            redibujar = false;
        }

        if (!juegoGanado) {
            // Calcular posición futura del jugador
            
            float intentoX = jugadorX;
            float intentoY = jugadorY;
            bool seMovio = false;

            if (teclas[ALLEGRO_KEY_LEFT]) { intentoX -= velocidad; seMovio = true; }
            if (teclas[ALLEGRO_KEY_RIGHT]) { intentoX += velocidad; seMovio = true; }
            if (teclas[ALLEGRO_KEY_UP]) { intentoY -= velocidad; seMovio = true; }
            if (teclas[ALLEGRO_KEY_DOWN]) { intentoY += velocidad; seMovio = true; }

            if (seMovio) {
                movimientosframe++; //Esta variable suma 2 por movimiento


            }

            if (movimientosframe ==2) { //Cuando esa varibale llega a 2, se suma 1 al número de movimientos
                movimientos++;
                movimientosframe = 0; //Se regresa a 0 para continuar evaluando
            }

            // Función auxiliar para verificar colisión en ambos ejes
            auto hayColision = [&](float x, float y) -> bool {
                int cx = (x - margenX) / tamCelda;
                int cy = (y - margenY) / tamCelda;

                if (cx < 0 || cx >= anchoLaberinto || cy < 0 || cy >= altoLaberinto)
                    return true;

                const Celda& celda = laberinto[cy][cx];
                float localX = x - (margenX + cx * tamCelda);
                float localY = y - (margenY + cy * tamCelda);
                float margen = 2.0f;

                // Colisión con muros de la celda actual
                if (localY - radioJugador < margen && celda.muroNorte) return true;
                if (localY + radioJugador > tamCelda - margen && celda.muroSur) return true;
                if (localX - radioJugador < margen && celda.muroOeste) return true;
                if (localX + radioJugador > tamCelda - margen && celda.muroEste) return true;

                // Colisión con muros de celdas adyacentes
                if (cx > 0) {
                    const Celda& izquierda = laberinto[cy][cx - 1];
                    if (localX - radioJugador < 0 && izquierda.muroEste) return true;
                }
                if (cx < anchoLaberinto - 1) {
                    const Celda& derecha = laberinto[cy][cx + 1];
                    if (localX + radioJugador > tamCelda && derecha.muroOeste) return true;
                }
                if (cy > 0) {
                    const Celda& arriba = laberinto[cy - 1][cx];
                    if (localY - radioJugador < 0 && arriba.muroSur) return true;
                }
                if (cy < altoLaberinto - 1) {
                    const Celda& abajo = laberinto[cy + 1][cx];
                    if (localY + radioJugador > tamCelda && abajo.muroNorte) return true;
                }

                return false;
                };

            // Verificar colisión por eje
            bool colisionX = hayColision(intentoX, jugadorY);
            bool colisionY = hayColision(jugadorX, intentoY);

            if (!colisionX) jugadorX = intentoX;
            if (!colisionY) jugadorY = intentoY;

    
            // Mantener al jugador dentro de los límites del laberinto
            if (jugadorX - radioJugador < margenX) jugadorX = margenX + radioJugador;
            if (jugadorX + radioJugador > margenX + anchoLaberinto * tamCelda) jugadorX = margenX + anchoLaberinto * tamCelda - radioJugador;
            if (jugadorY - radioJugador < margenY) jugadorY = margenY + radioJugador;
            if (jugadorY + radioJugador > margenY + altoLaberinto * tamCelda) jugadorY = margenY + altoLaberinto * tamCelda - radioJugador;

            // Verificar si llegó a la meta (centro del cuadrado rojo)
            bool enMetaX = (jugadorX >= metaPosX - tamMeta / 2 && jugadorX <= metaPosX + tamMeta / 2);
            bool enMetaY = (jugadorY >= metaPosY - tamMeta / 2 && jugadorY <= metaPosY + tamMeta / 2);
            if (enMetaX && enMetaY) {
                juegoGanado = true;
                tiempoSegundos = al_get_time() - tiempoInicio;
                salirPorVictoria = true; // Cambia la variable a verdadero

                // Reproducir sonido de victoria
                if (sonidoVictoria && !sonidoReproducido) {
                    al_play_sample(sonidoVictoria, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    sonidoReproducido = true;
                    al_rest(0.3); //Pausa para que el sonido se reproduzca
                }

                //Salir inmediatamente al ganar
                redibujar = false;
                break; // Salir del bucle del juego inmediatamente
            }
        }

        // Dibujar
        al_clear_to_color(al_map_rgb(40, 40, 40));

        // Dibujar laberinto
        for (int y = 0; y < altoLaberinto; y++) {
            for (int x = 0; x < anchoLaberinto; x++) {
                int posX = x * tamCelda + margenX;
                int posY = y * tamCelda + margenY;

                const Celda& celda = laberinto[y][x];

                // Fondo de la celda
                al_draw_filled_rectangle(posX, posY, posX + tamCelda, posY + tamCelda, al_map_rgb(210, 180, 140));
                
                float grosor = 2.0f;
                ALLEGRO_COLOR colorMuro = al_map_rgb(139, 69, 19); // Marrón

                // Dibujar muros
                if (celda.muroNorte)
                    al_draw_line(posX, posY, posX + tamCelda, posY, colorMuro, grosor);

                if (celda.muroSur)
                    al_draw_line(posX, posY + tamCelda, posX + tamCelda, posY + tamCelda, colorMuro, grosor);

                if (celda.muroEste)
                    al_draw_line(posX + tamCelda, posY, posX + tamCelda, posY + tamCelda, colorMuro, grosor);

                if (celda.muroOeste)
                    al_draw_line(posX, posY, posX, posY + tamCelda, colorMuro, grosor);
            }
        }

        // Dibujar celda de inicio (cuadrado verde claro en la primera celda)
        float inicioX = margenX + tamCelda / 2.0f;
        float inicioY = margenY + tamCelda / 2.0f;
        float tamInicio = tamCelda / 2.0f;

        al_draw_filled_rectangle(inicioX - tamInicio / 2, inicioY - tamInicio / 2,
            inicioX + tamInicio / 2, inicioY + tamInicio / 2,
            al_map_rgb(0, 255, 0)); // Verde claro
        al_draw_rectangle(inicioX - tamInicio / 2, inicioY - tamInicio / 2,
            inicioX + tamInicio / 2, inicioY + tamInicio / 2,
            al_map_rgb(0, 200, 0), 2); // Borde verde oscuro

        // Dibujar meta (cuadrado rojo en la celda final)
        al_draw_filled_rectangle(metaPosX - tamMeta / 2, metaPosY - tamMeta / 2,
            metaPosX + tamMeta / 2, metaPosY + tamMeta / 2,
            al_map_rgb(255, 0, 0));
        al_draw_rectangle(metaPosX - tamMeta / 2, metaPosY - tamMeta / 2,
            metaPosX + tamMeta / 2, metaPosY + tamMeta / 2,
            al_map_rgb(200, 0, 0), 2);

        // Dibujar jugador
        al_draw_filled_circle(jugadorX, jugadorY, radioJugador, al_map_rgb(255, 0, 0));

        // Mostrar mensajes
        if (juegoGanado) {
            al_draw_text(fuente, al_map_rgb(0, 255, 0), ventanaAncho / 2, 20, ALLEGRO_ALIGN_CENTER,
                "¡FELICIDADES! ¡GANASTE EL JUEGO!");
            al_draw_textf(fuente, al_map_rgb(255, 255, 255), ventanaAncho / 2, 50, ALLEGRO_ALIGN_CENTER,
                "Movimientos: %d - Tiempo: %.1f segundos", movimientos, tiempoSegundos);
            al_draw_text(fuente, al_map_rgb(255, 255, 255), ventanaAncho / 2, 80, ALLEGRO_ALIGN_CENTER,
                "Presiona ESC para salir");
        }
        else {
            /*
            al_draw_text(fuente, al_map_rgb(255, 255, 255), 10, 10, 0,
                "Laberinto Generado - Llega al cuadrado rojo - Presiona ESC para salir");
            al_draw_textf(fuente, al_map_rgb(255, 255, 255), 10, 30, 0,
                "Movimientos: %d", movimientos);
            al_draw_textf(fuente, al_map_rgb(255, 255, 255), 10, 50, 0,
                "Tiempo: %.1f segundos", al_get_time() - tiempoInicio);
            */
            int panelX = 20;
            int panelY = 20;
            int salto = 20;

            al_draw_text(fuente, al_map_rgb(255, 255, 255), panelX, panelY, 0,
                "Laberinto Generado");
            al_draw_text(fuente, al_map_rgb(255, 255, 255), panelX, panelY + salto, 0,
                "Objetivo: llegar al cuadrado rojo");
            al_draw_textf(fuente, al_map_rgb(255, 255, 255), panelX, panelY + salto * 2, 0,
                "Movimientos: %d", movimientos);
            al_draw_textf(fuente, al_map_rgb(255, 255, 255), panelX, panelY + salto * 3, 0,
                "Tiempo: %.1f s", al_get_time() - tiempoInicio);
        }

        al_flip_display();


        if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                redibujar = false;
            }

        }

       
    }
    

    al_destroy_event_queue(colaEventos);
    
    //if (sonidoVictoria) al_destroy_sample(sonidoVictoria);
    //al_uninstall_audio();

    // Guardar estadísticas reales
    if (juegoGanado) {
        guardarEstadistica(anchoLaberinto, altoLaberinto, movimientos, tiempoSegundos);
        std::cout << "Juego completado! Movimientos: " << movimientos << ", Tiempo: " << tiempoSegundos << " segundos\n";
    }
    else {
        tiempoSegundos = al_get_time() - tiempoInicio;
        std::cout << "Juego abandonado. Movimientos: " << movimientos << ", Tiempo: " << tiempoSegundos << " segundos\n";
    }

    //Guardar datos de partida
    DatosPartida datos;
    datos.movimientos = movimientos;
    datos.tiempo = tiempoSegundos;
    datos.anchoLaberinto = anchoLaberinto;
    datos.altoLaberinto = altoLaberinto;

    return datos; //Retorno de datos

    
}