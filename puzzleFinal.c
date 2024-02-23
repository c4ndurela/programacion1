#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE 4

int emptyX, emptyY; // variables donde se va a almacenar la coordenada del casillero vacío
void mostrarTablero(int tablero[SIZE][SIZE]);  
int moverVacio(int tablero[SIZE][SIZE], int xfinal, int yfinal);
int mover(int tablero[SIZE][SIZE], char mov);
int imprimeError();
int salirDelJuego();
int recibeApuesta();
int apuestaMaxMin();
void mostrarPuntajes(int partidasJugadas, int puntos[]);
void bienvenida();

int puntajes[10];
int partidaGanadora[1][SIZE][SIZE] = {//partida con las fichas en orden
    {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12,}, {13, 14, 15, 0}}};


bool esTableroGanador(int tablero[SIZE][SIZE]) { /* recibe como arg la matriz del tablero,
                                                 compara el tablero con la partida ganadora y si son iguales devuelve true,
                                                 de lo contrario false. */
    for (int i = 0; i < SIZE; i++) {//recorre cada celda 
        for (int j = 0; j < SIZE; j++) {
            if (tablero[i][j] != partidaGanadora[0][i][j]) {//la compara con la del tablero ganador
                return false; // Si encuentra una diferencia, el tablero no es ganador
            }
        }
    }
    return true; // Si no hay diferencias, el tablero es ganador
}


int partidas[11][SIZE][SIZE] = { //array 3d tableros de 4x4 con 11 soluciones posibles 
    {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 0, 15}},
    {{11, 1, 10, 4}, {8, 3, 2, 12}, {7, 6, 15, 0}, {9, 5, 14, 13}},
    {{7, 12, 14, 1}, {0, 5, 13, 2}, {11, 8, 15, 3}, {6, 4, 10, 9}},
    {{11, 1, 3, 6}, {8, 13, 0, 9}, {7, 12, 2, 15}, {5, 4, 14, 10}},
    {{1, 6, 11, 12}, {5, 2, 4, 15}, {0, 3, 13, 10}, {8, 9, 14, 7}},
    {{3, 6, 1, 14}, {12, 15, 8, 5}, {0, 7, 11, 9}, {4, 2, 13, 10}},
    {{15, 10, 14, 4}, {1, 8, 3, 5}, {2, 9, 11, 12}, {13, 0, 7, 6}},
    {{13, 15, 9, 11}, {6, 14, 2, 7}, {0, 10, 5, 8}, {12, 3, 4, 1}},
    {{15, 14, 5, 2}, {3, 13, 0, 10}, {1, 12, 7, 6}, {11, 4, 8, 9}},
    {{9, 2, 0, 3}, {5, 4, 7, 15}, {10, 8, 6, 14}, {1, 11, 12, 13}},
    {{5, 6, 15, 11}, {9, 0, 2, 4}, {12, 3, 13, 7}, {14, 1, 10, 8}}};

int main() {//
    int movimientos = 0; // inicializa la cant. de movimientos de la partida
    int numerodepartida = 0;//inicializa el número de partida
    int apu;//almacena la apuesta del usuario
    char ch; // se almacena el movimiento con w,a,s,d
    initscr(); // inicia el ncurses mode
    bienvenida();
    raw();     // lee los caracteres uno a uno y no linea a linea
    noecho(); //funcion de ncurses para que no muestre las teclas que se presionan
    for (numerodepartida = 0; numerodepartida <= 10; numerodepartida++) {//bucle principal
        echo();
        apu = apuestaMaxMin();//se llama a apuestamaxmin para que el usuario ingrese la apuesta
        movimientos = 0; //reinicia los movimientos para cada nueva partida
        refresh();
        noecho();
        clear();
        while (1) { //el while (1) no termina hasta que encuentra un break o condición de salida
            printw("Partida n° %d \n", numerodepartida + 1); //+1 porque como incia en 0 queremos que ennumere desde el 1
            printw("Movimientos: %d\n", movimientos); // imprime la cantidad de movimientos actualizada al momento
            printw("Tu apuesta es de %d \n", apu);
            printw("Realice un movimiento en el tablero: \n");
            mostrarTablero(partidas[numerodepartida]);
            printw("Para cambiar de partida presione 'm'.\n\nPara abandonar la partida presione 'p'.\n");
            ch = getch(); //lee un caracter sin necesidad de apretar el enter
            refresh(); //actualiza la pantalla y muestra los cambios en pantalla
            
            if (ch == 'p') {//pesionando la p salimos del juego
                salirDelJuego();
            }
            movimientos = mover(partidas[numerodepartida], ch) + movimientos;//se llama a mover con 2 argumentos: tablero con partidas^[],
                        //con la var numerodepartida y ch con el caracter que ingresa el usuario p mover, devuelve la cantidad de movimientos
                        // realizados y se acumula al total de movimientos en 'movimientos'
            if (movimientos >= 999) {//sale del bucle si supera la cantidad de movimientos
                puntajes[numerodepartida] = recibeApuesta (apu, movimientos);//si se excede calcula el puntaje segun la apuesta y movimientos con la func recibeApuesta
                break;
            }
            if (esTableroGanador(partidas[numerodepartida])) {//llama a la funcion estableroganador con el tablero actual como argumento y comprueba si son identicos (true)
                clear();
                refresh(); 
                puntajes[numerodepartida] = recibeApuesta (apu, movimientos);//se calculan los puntajes de acuerdo a la apuesta y movimientos realizados,
                //el valor dvuelto se almacena en el arreglo partidas para al partida actual
                printw("Felicitaciones! Ganaste! Tu puntaje es %d \n\n", puntajes[numerodepartida]);  
                mostrarTablero(partidaGanadora[0]);
                printw("\n");
                break;
            } 

        }
        mostrarPuntajes (numerodepartida, puntajes);
        salirDelJuego(); //damos la opcion de continuar jugando o salir de la partida

        movimientos = 0;
    } 
    endwin();
    return 1;
}

void mostrarTablero(int tablero[SIZE][SIZE]) {//función que no devuelve ningun valor, recibe como argumeneto un array bidimencional de 4x4
    int i, j;//indices de los bucles de las filas y columnas
    for (i = 0; i < SIZE; i++) {//recorre las filas
        for (j = 0; j < SIZE; j++) {//recorre las columnas
            if (tablero[i][j] == 0) {//comprueba si el casillero es igual a cero imprime vacío
                printw("|   ");
                emptyX = j;//variables para ubicar el casillero vacío
                emptyY = i;
            } else {
                printw("|%2d ", tablero[i][j]);
            }
        }
        printw("|\n");
    }
}
void bienvenida(){//funcion que da la bienvenida al juego y muestra las reglas.
    char opciones;//en esta variable guardamos la opcion que elije el jugador
    printw("BIENVENIDO A PUZZLE 15! \nPara ingresar a las reglas del juego presione 'r' (rules) seguido del enter.\n");
    printw("Para comenzar una nueva partida presione 'n'(new) seguido del enter.\n");
    printw("Para salir ahora del juego presione 'e'(exit) seguido del enter.\n");
    scanw(" %c", &opciones);
       if(opciones == 'r'){ 
            printw("Reglas: El objetivo del juego es mover las piezas del tablero de modo que todas queden ordenadas del 1 al 15, de la siguiente manera: \n");
            mostrarTablero(partidaGanadora[0]);
            printw("\nPara mover las piezas al espacio vacío, utilice las teclas en minúsculas: \n");
            printw("'a' mueve hacia la izquierda \n");
            printw("'d' mueve hacia la derecha \n");
            printw("'w' mueve hacia arriba \n");
            printw("'s' mueve hacia abajo \n\n");
            printw("Puede salir del juego en cualquier momento presionando la tecla 'p' o comenzar una nueva partida presionando 'm'.\n\n");
        }
       else if(opciones == 'n'){

        }
       else if(opciones == 'e'){ //Opcion para salir del juego
         endwin(); //finaliza la ventana de la terminal (ncurses)
         exit(0); //el 0 finaliza el programa sin errores, el programa se detiene y regresa el control al sistema operativo
        }
       else {//si presiona cualquier otra tecla limpia la pantalla, insiste con las teclas validas, y vuelve al mensaje de bienvenida
            clear();
            printw("Presione 'r', 'n' o 'e'.\n");
            refresh();
            napms(1500);//funcion que pausa la ejecución 1500 milisegundos (1,5 seg)
            clear();
            bienvenida();
        }

}

int imprimeError() {//función general para cuando se mueve la ficha fuera del tablero (bordes)
    clear();
    printw("\n Imposible realizar este movimiento \n");
    return 0;
}

int mover(int tablero[SIZE][SIZE],char mov) { // tablero es el tablero que estoy usando, mov es para
                      // donde vamos a mover
    switch (mov) {
    case 's'://cuando presionas la tecla 's' el casillero vacío se mueve para arriba 
            //generando la ilusion de que el numero que está arriba, baja.
        if (emptyY <= 0) {//si el casillero vacío está sobre la fila 0 imprime error porque no puede moverse hacia arriba
            return imprimeError();
        } else {
            return moverVacio(tablero, emptyY - 1, emptyX);
        }
    case 'w':
        if (emptyY >= 3) {
            return imprimeError();
        } else {
            return moverVacio(tablero, emptyY + 1, emptyX);
        }
    case 'd':
        if (emptyX <= 0) {
            return imprimeError();
        } else {
            return moverVacio(tablero, emptyY, emptyX - 1);
        }
    case 'a':
        if (emptyX >= 3) {
            return imprimeError();
        } else {
            return moverVacio(tablero, emptyY, emptyX + 1);
        }
    case 'm'://aprentando la tecla p se finaliza la partida actual y comienza la siguiente
        clear();
        printw("Nueva partida.\n");
        refresh();  // Actualiza la pantalla para mostrar el mensaje
        napms(1500);  // Espera 1.5 segundos 
        clear();
        return 999; /* cuando ingresa la tecla 'p' se adicionan 999 movimientos y se termina la partida como un exit */
    default:
        clear();
        printw("Para moverse por el tablero utilice alguna de las siguientes teclas en minúscula: w,a,s,d \n");
    }
    return 0;
}

int moverVacio(int tablero[SIZE][SIZE], int yfinal, int xfinal) {//funcion para realizar un movimiento a la celda vacia modificando el tablero
    tablero[emptyY][emptyX] = tablero[yfinal][xfinal];//se asigna la posición final a la posición actual vacía entonces "mueve" la ficha a la posición final.
    tablero[yfinal][xfinal] = 0;//se le asiga 0 a la posición donde estaba la ficha movida para que quede 'vacía', 
    emptyY = yfinal;//Se actualiza la posición de la ficha vacía a la posición final
    emptyX = xfinal;
    clear();
    return 1;//devuelve 1 para indicar q el movimiento se realizo con exito
}

int recibeApuesta(int apu, int mov) {//La función toma dos parámetros: apu(la apuesta realizada) y mov (cant de movimientos)
    int puntos = 0; //variable para almacenar los puntos
    if (mov == apu) {//si la cant de movimientos es igual a la apuesta, recibe 1000 ptos
        puntos = 1000;
    } else if (mov >= apu - 10 && mov <= apu) {//si los movimientos están entre 10 menos de la apuesta y la apuesta, recibe 1200
        puntos = 1200;
    } else if (mov >= apu && mov <= apu + 10) {//si los movimientos están entre la apuesta y 10+, recibe 500 ptos
        puntos = 500;
    } else if (mov <= apu - 10) {//si los movimientos son menores a la apuesta -10, recibe 1500
        puntos = 1500;
    } else {
        puntos = 0;//cualquier otro caso
    }
    return puntos;
}
int apuestaMaxMin(){
    int apuesta;
    while (1) {
        printw("Realice una apuesta: en cuántos movimientos cree que ganará la partida? Ingrese un número entre 40 y 200:\n");
        scanw("%d", &apuesta);
        refresh();
        if (apuesta >= 40 && apuesta <= 200) {
            printw("Su apuesta es: %d \n", apuesta);
            break;
        } else {
            printw("La apuesta debe estar entre 40 y 200. Por favor, ingrese un número válido.\n");
            refresh();
        }
        
    } 
    printw("Si logra completar el puzzle entre %d y %d movimientos obtiene 1200 puntos\n", apuesta - 10, apuesta - 1);
    printw("Si logra completar el puzzle entre %d y %d movimientos obtiene 500 puntos\n", apuesta + 1, apuesta + 10);
    printw("Si logra completar el puzzle en exactamente %d movimientos obtiene 1000 puntos\n", apuesta);
    printw("Si logra completar el puzzle en menos de %d movimientos obtiene 1500 puntos\n", apuesta - 10);
    refresh();//Refrescamos la consola.
    napms(7000); //muestra los mensajes durante 7 segundos y luego empeiza la partida
    return apuesta;
}
    
void mostrarPuntajes(int partidasJugadas,int puntos[]){ //funcion que toma 2 argumentos (partidasjudas y el array puntos)e imprime en pantalla los puntos obtenidos en cada partida 
    int x = 0; //iniciamos la var x en cero para usarla de contador en el bucle
    for(x = 0; x <= partidasJugadas; x++){//bucle que recorre las partidas jugadas
        printw("Partida n° %d, Puntos: %d \n", x + 1, puntos[x]);//imprime el n de partida y los puntos obtenidos en cada una (x+1 pq se inicializa en 0)
    }
}

int salirDelJuego() {//función para salir del juego en cualquier momento de la partida
    char ex; //declaramos esta variable ex de tipo char para almacenar la tecla que presiona el usuario
    printw("Presione 'p' para salir del juego. Presione cualquier otra tecla para seguir jugando. \n ");
    ex = getch();//se guarda lo que presiona el usuario en la var ex
    if (ex == 'p'){//si la tecla es p, se finaliza el programa
        endwin(); //finaliza ncurses
        exit(0); //finaliza el programa
    } else {//si es cualquier otra letra muestra mensaje en pantalla y continua la partida
        printw("Continúa jugando!\n");
        refresh(); // Actualiza la pantalla
        napms(2000);  // función de ncruses q pausa durante 2 segundos
    }
    return 0;//retorna 0 pq se ejecuto correctamente
}
