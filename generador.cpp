// generador.cpp
#include "generador.h"
#include <iostream>    // Para std::cout
#include <cstdlib>     // Para std::srand, std::rand
#include <ctime>       // Para std::time
#include <stack>       // Para std::stack
#include <utility>     // Para std::pair
#include <allegro5/allegro.h>//Para inicializar allegro
#include <allegro5/allegro_image.h>//Para poder dibujar con allegro
#include "celda.h"
// Función para quitar muros entre dos celdas adyacentes
void quitarMuroEntre(int x1, int y1, int x2, int y2, std::vector<std::vector<Celda>>& laberinto) {

    Celda& celda1 = laberinto[y1][x1];//Se guarda la dirección de la celda 1
    Celda& celda2 = laberinto[y2][x2];//Se guarda la dirección de la celda 2

    
    if (x2 == x1 + 1) {//Si las celdas estan pegadas o cerca se eliminan los muros. 
        
        celda1.muroEste = false;  //eliminación de muros
        celda2.muroOeste = false; //eliminación de muros
    }
    else if (x2 == x1 - 1) {//Condicion igual solo que aquí se comparan valores de forma diferente.
        
        celda1.muroOeste = false; //eliminación de muros
        celda2.muroEste = false;  //eliminación de muros
    }
    else if (y2 == y1 + 1) {//Condicion igual solo que aquí se comparan valores de forma diferente.
        
        celda1.muroSur = false;  //eliminación de muros 
        celda2.muroNorte = false; //eliminación de muros
    }
    else if (y2 == y1 - 1) {//Condicion igual solo que aquí se comparan valores de forma diferente.
        
        celda1.muroNorte = false; //eliminación de muros
        celda2.muroSur = false;  //eliminación de muros
    }
}
//Función para obtener vecinos no visitados.
std::vector<int> obtenerVecinosNoVisitados(int x, int y, std::vector<std::vector<Celda>>& laberinto) {
	std::vector<int> direccionesDisponibles;// Vector para almacenar direcciones disponibles
	int alto = laberinto.size();// Altura del laberinto
	int ancho = laberinto[0].size();// Ancho del laberinto

	// Direcciones: {0 = Norte, 1 = Sur, 2 = Este, 3 = Oeste}
	int dx[] = { 0, 0, 1, -1 };//Genera los cambios en x para cada dirección. Recordar que los ultimos 2 son Este y Oeste. 
	int dy[] = { -1, 1, 0, 0 };//Genera los cambios en y para cada dirección. Recordar que los primeros 2 son Norte y Sur.

	for (int i = 0; i < 4; i++) {// Itera sobre las 4 direcciones
		int nuevoX = x + dx[i];// Calcula nueva posición x, esta posición cambia según el indice i.
		int nuevoY = y + dy[i];// Calcula nueva posición y, esta posición cambia según el indice i.

		//Primero se verifica que la nueva posición esté dentro de los límites del laberinto.
        if (nuevoX >= 0 && nuevoX < ancho && nuevoY >= 0 && nuevoY < alto) {
			//Luego se verifica si la nueva celda no ha sido visitada.
            if (!laberinto[nuevoY][nuevoX].visitada) {
				direccionesDisponibles.push_back(i);
			}// Si no ha sido visitada, se agrega la dirección al vector, es decir se agrega la dirección disponible. Esto se hace con un push_back.
        }
    }

    return direccionesDisponibles;
}


//Recibe la matriz del laberinto y debe modificar los muros para crear un camino válido.
void generarLaberinto(std::vector<std::vector<Celda>>& laberinto) {
	

	// Inicializar la semilla para la generación aleatoria. Evitando patrones repetitivos osea laberintos iguales.
    std::srand(std::time(0));
	//Se obtiene las dimensiones del laberinto para recorrerlo.
    int alto = laberinto.size();
    int ancho = laberinto[0].size();
	//Pila para el algoritmo de backtracking. 
    std::stack<std::pair<int, int>> pila;

	//Se elige una celda inicial, en este caso se usara la celda (0,0) para empezar.
	int xInicial = 0; //se define la posición inicial en x
	int yInicial = 0;//se define la posición inicial en y

	laberinto[yInicial][xInicial].visitada = true;//significa que la celda inicial ya fue visitada
	pila.push({ xInicial, yInicial });//la celda ya visitada se agrega a la pila

	while (!pila.empty()) {//Bucle principal que sirve para generar el laberinto, sigue hasta que la pila esté vacía.
        
		int xActual = pila.top().first;//obtenemos la posición actual en x, que es el elemento superior de la pila, osea el último elemento agregado.
		int yActual = pila.top().second;//obtenemos la posición actual en y, que es el segundo elemento superior de la pila.


		std::vector<int> vecinos = obtenerVecinosNoVisitados(xActual, yActual, laberinto);//obtenemos los vecinos no visitados de la celda actual.
		if (!vecinos.empty()) {//si hay vecinos no visitados.
			// Se elige uno de los vecinos al azar.
            int direccionElegida = vecinos[std::rand() % vecinos.size()];

			// Para calcular las coordenadas del vecino elegido.
            int dx[] = { 0, 0, 1, -1 };
            int dy[] = { -1, 1, 0, 0 };

			//Se obtienen las coordenadas del vecino elegido.
            int nuevoX = xActual + dx[direccionElegida];
            int nuevoY = yActual + dy[direccionElegida];


            quitarMuroEntre(xActual, yActual, nuevoX, nuevoY, laberinto);//Función para quitar los muros(quitar los espacios entre las celdas)

			//Cuanto termina el proceso de elegir el vecino, se marca como visitado y se agrega a la pila.
            laberinto[nuevoY][nuevoX].visitada = true;
            pila.push({ nuevoX, nuevoY });

            
        }
        else {
			pila.pop(); // Solo se retrocede si no hay vecinos no visitados.
        }
    }
   
}
