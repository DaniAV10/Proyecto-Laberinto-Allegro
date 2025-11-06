//ProyectoLaberintoAllegro
/*Integrantes:
-Daniel Alejandro Arrieta Víquez
-Ronny Espinoza Cordero
-Cristhofer Herrera
*/


//main.cpp

//1. Bibliotecas de Allegro y encabezados del proyecto
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <cstdio> //Para imprimir errores
#include "menu.h"           //Menú principal
#include "laberinto.h"      //Estructura del laberinto
#include "configuracion.h"  //Pantalla de configuración
#include "estadisticas.h" //Pantalla de estadísticas
#include "generador.h" //Vincular con función que reciba la matriz laberinto
#include "juego.h" //Vincular con juego
#include "victoria.h" //Obtiene la pantalla de victoria


int main() {
    static DatosPartida datosVictoria; //Variable global temporal para guardar los datos de partida
    //2. Inicialización de Allegro y sus componentes
    if (!al_init()) {
        fprintf(stderr, "Error al inicializar Allegro.\n"); //En caso de que de error
        return -1;
    }

    al_install_keyboard();       //Para capturar teclas
    al_init_font_addon();        //Para usar fuentes
    al_init_ttf_addon();         //Para fuentes TTF
    al_init_primitives_addon();  //Para dibujar líneas y formas
    al_init_image_addon();       // Para cargar el ícono

    //3. Crear ventana, cola de eventos y fuente
    ALLEGRO_DISPLAY* display = al_create_display(800, 600); // Tamaño ventana
    if (!display) {
        fprintf(stderr, "Error al crear la ventana.\n");
        return -1;
    }

    ALLEGRO_BITMAP* icono = al_load_bitmap("iconos/icon.ico");
    if (!icono) {
        icono = al_load_bitmap("iconos/icon.png");
    }
    if (icono) {
        al_set_display_icon(display, icono);
        printf("Ícono cargado correctamente\n");
    }
    else {
        printf("No se pudo cargar el ícono\n");
    }

    // 4. Configurar título de la ventana
    al_set_window_title(display, "Laberinto Allegro");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    if (!queue) {
        fprintf(stderr, "Error al crear la cola de eventos.\n");
        al_destroy_display(display);
        return -1;
    }

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));

    //Fuente integrada, la que está por defecto
    ALLEGRO_FONT* fuente = al_create_builtin_font();

    //4. Estado inicial del programa
    EstadoPrograma estado = MENU_PRINCIPAL;

    //5. Mostrar el menú al iniciar
    mostrarMenu(fuente);
    al_flip_display();

    //6. Bucle principal del programa
    while (estado != SALIR) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(queue, &evento); //Espera una tecla

        switch (estado) {
        case MENU_PRINCIPAL: {
            //Muestra el menú principal con las opciones disponibles
            estado = manejarMenu(evento); //Cambia estado según tecla
            if (estado == MENU_PRINCIPAL) {
                mostrarMenu(fuente); //Redibuja si se sigue en el menú
                al_flip_display(); // Icono del programa
            }
            break;
        }

        case CONFIGURACION_LABERINTO: {
            //Pantalla interactiva para elegir tamaño del laberinto
            int ancho = 10;
            int alto = 10;
            mostrarConfiguracion(fuente, ancho, alto);
            al_flip_display();

            bool confirmado = false;
            while (!confirmado) {
                //Espera eventos de teclado para modificar ancho y alto
                ALLEGRO_EVENT eventoConfig;
                al_wait_for_event(queue, &eventoConfig);

                confirmado = manejarConfiguracion(eventoConfig, ancho, alto);
                mostrarConfiguracion(fuente, ancho, alto);
                al_flip_display();
            }
            
            //Inicializa la matriz del laberinto con celdas vacías
            inicializarLaberinto(ancho, alto);

            generarLaberinto(laberinto); //Llamar al generador de laberintos

            //Cambia al estado de juego, se encarga de la visualización y movimiento
            estado = JUEGO;
            break;
        }

        case ESTADISTICAS: {
            mostrarEstadisticasEnPantalla(fuente); //Se muestran las estadísticas
            al_flip_display();
            estado = MENU_PRINCIPAL;
            break;
        }
    
        case RESUMEN_GLOBAL: {
            mostrarResumenEstadisticasGlobales(fuente);
            estado = MENU_PRINCIPAL;
            break;
        }

        case VICTORIA: {
            mostrarPantallaVictoria(fuente, datosVictoria);
            al_flip_display();

            bool decisionTomada = false;

            while (!decisionTomada) {
                ALLEGRO_EVENT eventoVictoria;
                al_wait_for_event(queue, &eventoVictoria);

                int opcion = manejarVictoria(eventoVictoria);
                if (opcion == 1) {
                    estado = CONFIGURACION_LABERINTO;
                    decisionTomada = true;
                }
                else if (opcion == 2) {
                    estado = MENU_PRINCIPAL;
                    decisionTomada = true;
                }
                else if (opcion == 3) {
                    estado = SALIR;
                    decisionTomada = true;
                }

                //Redibujar
                if (!decisionTomada) {
                    mostrarPantallaVictoria(fuente, datosVictoria);
                    al_flip_display();
                }
            }
            break;
        }

        case JUEGO: {
            //Llama al módulo de juego
            DatosPartida datos = ejecutarJuego(fuente, display);
            datosVictoria = datos;
            estado = VICTORIA;
            break;
        }

        default:
            break;
        }
    }

    //7. Liberar recursos
    if (icono) al_destroy_bitmap(icono);
    al_destroy_display(display);
    al_destroy_event_queue(queue);
    al_destroy_font(fuente);

    return 0;
}
