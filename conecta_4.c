/*
 *
 * Programa hecho para poder jugar conecta 4 entre 2 humanos o para jugarlo contra una IA basada en minimax
 * autores: 
 * Jesus David Rodriguez
 * Wilter Diaz Lopez 
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


//Macros para imprimir a color
#define ROJO "\e[0;31m"
#define AMARILLO "\e[0;33m"
#define REINICIO "\e[0m"

//Estructura de datos para los jugadores
struct {
	char *nombre;
	char tipo;
	int color;
	
} jugador[2] = {{"",'X',1},{"",'O', 2}};

//Valores de FILAS y COLUMNAS se le daran a elegir al usuario
int FILAS = 6, COLUMNAS = 7, LINEA = 4, PROFUNDIDAD = 7;

//Variable donde se almacenara la jugada devuelta por minimax
int jugada_IA;


//max: retorna el maximo de 2 enteros
int max(int a, int b){
	
	if(a > b) {
		return a;
	}
	
	return b;
	
}

//min: retorna el minimo de 2 enteros
int min(int a, int b){
	
	if(a < b){
		return a;
	}
	
	return b;
	
}

//copiar_matriz: genera una copia de un tablero dado, esta funcion
//sera utilizada para calcular las posiciones en el minimax sin alterar
//el tablero original
void copiar_matriz(char tablero[][COLUMNAS], char copia[][COLUMNAS]){
	
	register int i, j;
	
	//Se recorre el arreglo original
	//y se copia en el otro
	for(i = 0; i < FILAS; i++){
		for(j = 0; j < COLUMNAS; j++){
			copia[i][j] = tablero[i][j];
        }
    }
}


//jugada_valida: verifica si quedan espacios en una
//columnas del tablero
bool jugada_valida(int columna, char tablero[][COLUMNAS]){

	register int i; 
	
	for(i = FILAS-1; i >= 0; i--){
		if(tablero[i][columna]=='_'){
			return true;
		}
	}
	
	//Jugada invalida
	return false;
}


//verifica_tablero_lleno: retorna 1 si el tablero esta lleno, 0 sino
int verifica_tablero_lleno(char tablero[][COLUMNAS]) {
  
  register int i;
  
  //Verifica toda la primera fila para ver si quedan espacios
  
  for (i = 0; i < COLUMNAS; i++) {
    //Si queda un espacio retorna falso
    if (tablero[0][i] == '_') {
      return 0;
    }
  }
  
  //No quedan espacios
  return 1;
}

//Busca patrones en 4 espacios pasados
int busca_patrones(char cuarteto[]){
    int puntaje = 0;
    int propias = 0;
    int oponente = 0;
    int vacias = 0;

	register int i;
	
	//Se verifica en cuarteto dado
	//cantidaad de piezas propias, del oponente
	//y espacios vacios
    for(i = 0; i < LINEA; i++){
	    if(cuarteto[i] == 'O')
	    	propias++;
	    else if(cuarteto[i] == '_')
	    	vacias++;
	    else if(cuarteto[i] == 'X')
	    	oponente++;
	}
	
	//Asigna puntos segun el patron econtrado
	if(propias == 4)
		puntaje += 150;
	else if (oponente == 4)
		puntaje += 100;
	else if(propias ==3 && vacias==1)
		puntaje +=5;
	else if(propias==2 && vacias==2)
		puntaje += 2;

	else if(oponente==3 && vacias==1)
		puntaje -= 4;

    return puntaje;
}

//evaluacion: asigna puntos a la posicion que 
//tenga el tablero
int evaluacion(char tablero[][COLUMNAS]){
    int puntaje = 0;
    int piezas_centrales = 0;
	
	//arreglos auxiliares
	char arreglo_filas[COLUMNAS], cuarteto[LINEA], arreglo_columnas[FILAS], arreglo_diagonal[LINEA];
	
	register int i, j, k;
	
    //Prioridad a columna central
    for(i = 0; i < FILAS; i++){
    	if(tablero[i][COLUMNAS>>1]=='O'){
    		puntaje += 3;
    	}
    }

    //Puntos de evaluacion horizontal
    for(i = FILAS - 1; i >= 0; i--){
    

		//Lleno fila auxiliar
    	for(j = 0; j < COLUMNAS; j++){
    		arreglo_filas[j] = tablero[i][j];
    	}
		
    	//Se llena cuarteto la filas
		for(j = 0; j < COLUMNAS - 3; j++){

			for(k = 0; k < LINEA; k++){
				cuarteto[k] = arreglo_filas[k + j];
			}
			//Se evalua el cuarteto de la fila
			//Y su evaluacion se suma al puntaje
			puntaje += busca_patrones(cuarteto);
		}
	}

	//Puntos de evaluacion vertical
    for(i = 0; i < COLUMNAS; i++){
    	
    	//Se llena columna auxiliar
    	for(j = 0; j < FILAS; j++){
    		arreglo_columnas[j] = tablero[j][i];
    	}
		
    	//Se llena cuarteto de la columna
		for(j = FILAS - 1; j >= 3; j--){

			for(k = 0; k < LINEA; k++){
				cuarteto[k] = arreglo_columnas[k + j];
			}
			
			//se evalua el cuarteto de la columnas
			//y su evaluacion se le suma al puntaje
			puntaje += busca_patrones(cuarteto);
		}
	}
	
    //Se verifica diagonal ascendente 
    for(i = FILAS - 1; i >= 3; i--){
    	

		//Se llena diagonal auxiliar
    	for(j = 0; j < COLUMNAS - 4; j++){
			
   
			//Se llena cuarteto de diagonal auxiliar
    		for(k =0; k < LINEA; k++){
    			arreglo_diagonal[k] = tablero[i - k][j + k];
    		}
			
			//Se evalua el cuarteto de la diagonal
			//y su evaluacion se le suma al puntaje		
    		puntaje += busca_patrones(arreglo_diagonal);
    	}
	}
	
	//Se verifica diagonal descendente
    for(i = FILAS - 1; i >= 3; i--){
   
    	//Se llena diagonal auxiliar
    	for(j = 3; j < COLUMNAS; j++){
			
    		//Se llena cuarteto de la diagonal
			for(k = 0; k < LINEA; k++){
    			arreglo_diagonal[k] = tablero[i - j][j + k];
    		}
			
			//Se evalua el cuarteto de la diagonal
			//y su evaluacion se le suma al puntaje				
    		puntaje += busca_patrones(arreglo_diagonal);
    	}
	}
	
	//Resultado de la evaluacion
    return puntaje;
}


//imprime_color: imprime cada ficha con el color que le toca
void imprime_color(int color) {
	
	//Imprime color correspondiente
	//a codigo dado
	if (color == 1) {
		printf(ROJO "O" REINICIO);
	} else if (color == 2) {
		printf(AMARILLO "O" REINICIO);
	}
}


//inicializacion: inicializa el tablero
void inicializacion(char tablero[][COLUMNAS]){
	
	register int i, j;
	
	//Llena todo el arreglo con el caracter '_'
	for(i = 0; i < FILAS; i++){
		for(j = 0; j < COLUMNAS; j++){
			tablero[i][j] = '_';
        }
    }
} 

//imprime_tablero; imprime el tablero y cada pieza dentro de el
//con su color correspondiente ayudandose con la funcion imprime_color
void imprime_tablero(char tablero[][COLUMNAS]){
	printf("\n");

	register int i, j;
	
	for(i = 1; i <= COLUMNAS; i++){
		printf("|%d", i);
	}
	printf("|\n");
	printf("\n");
	//Se recorre tablero
	for(i = 0; i < FILAS; i++){
		for(j = 0; j < COLUMNAS; j++){
			printf("|");
			//Se imprime el color que le corresponda a cada ficha
			if (tablero[i][j] == jugador[0].tipo) {
				imprime_color(jugador[0].color);
				
			} else if (tablero[i][j] == jugador[1].tipo) {
				imprime_color(jugador[1].color);
			} else{
				printf("-");
			}
        }
        printf("|\n");
	}

	printf("\n");
}


//introduce_jugada: Marca jugada en columna dada
void introduce_jugada(char tablero[][COLUMNAS],int columna, bool jugador){
	
	register int i;
	
	
	//Se introduce pieza dada en la columna 
	//correspondiente
	for(i = FILAS - 1; i >= 0; i--){
		if(tablero[i][columna]=='_')  {
			if(jugador){
				tablero[i][columna]='O';
				break;
			}
			else{
				tablero[i][columna]='X';
				break;
			}
		}
	}
	
	
	
}

//verifica_jugada_ganadora: retorna 0 si no hay posicion de victoria
//en el tablero, 1 o 2 dependiendo de la pieza que resulte ganadora
int verifica_jugada_ganadora(char tablero[][COLUMNAS]){
	
	register int i, j;
	
	//Se verifica verticalmente
	for(i = FILAS - 1 ; i >= FILAS - 3; i--){
		for(j = 0; j < COLUMNAS ; j++){
			
			if(tablero[i][j]=='X' && tablero[i -1][j]=='X' && tablero[i - 2][j]=='X' && tablero[i - 3][j]=='X'){
				return 1;
			}
			else if(tablero[i][j]=='O' && tablero[i - 1][j]=='O' && tablero[i - 2][j]=='O' && tablero[i - 3][j]=='O'){
				return 2;
			}
		}
	}

	//Se verifica horizontalmente
	for(i = FILAS - 1; i >= 0; i--){
		for(j = 0; j < COLUMNAS - 3; j++){
			if(tablero[i][j]=='X' && tablero[i][j + 1]=='X' && tablero[i][j + 2]=='X' && tablero[i][j + 3]=='X'){
				return 1;
			}
			else if(tablero[i][j]=='O' && tablero[i][j + 1]=='O' && tablero[i][j + 2]=='O' && tablero[i][j + 3]=='O'){
				return 2;
			}
		}
	}

	//Se verifica diagonal de abajo hacia arriba
	for(i = FILAS - 1; i >= FILAS - 3; i--){
		for(j = 0; j < COLUMNAS - 3; j++){
			if(tablero[i][j]=='X' && tablero[i - 1][j + 1]=='X' && tablero[i - 2][j + 2]=='X' && tablero[i - 3][j + 3]=='X'){
				return 1;
			}
			else if(tablero[i][j]=='O' && tablero[i - 1][j + 1]=='O' && tablero[i - 2][j + 2]=='O' && tablero[i - 3][j + 3]=='O'){
				return 2;
			}
		}
	}

	//Se verifica diagonal de arriba hacia abajo
	for(i = FILAS - 1; i >= FILAS - 3; i--){
		for(j = 3; j < COLUMNAS; j++){
			if(tablero[i][j]=='X' && tablero[i-1][j-1]=='X' && tablero[i-2][j-2]=='X' && tablero[i-3][j-3]=='X'){
				return 1;
			}
			else if(tablero[i][j]=='O' && tablero[i-1][j-1]=='O' && tablero[i-2][j-2]=='O' && tablero[i-3][j-3]=='O'){
				return 2;
			}
		}
	}
	
	//No hay ganador 
	return 0;
}

//minimax: Algoritmo que usa la IA para tomar decisiones basandose en una funcion de evaluacion
int minimax(int profundidad, char tablero[][COLUMNAS], bool maximizacion, int alfa, int beta) {
	
	int m_valor, m_jugada = rand() % COLUMNAS, val, i;

	
	//Si se llego a la profundidad maxima o el tablero esta lleno evaluar posicion
	if(profundidad == 0 || verifica_tablero_lleno(tablero) == 1){

		return evaluacion(tablero);
			
	}

	//retornar victoria
	if(verifica_jugada_ganadora(tablero) == 2){
		return(1000000);
	} else if(verifica_jugada_ganadora(tablero) == 1){
		return(-1000000);
	}
		
   	
   	//Modo de maximizacion de evaluacion
   	if(maximizacion) {
   		m_valor = -10000;
		//Inicializacion random
   		m_jugada = rand() % COLUMNAS; 
		
		//Si la jugada random no es valida se cambia hasta que lo sea
		while (!jugada_valida(m_jugada, tablero)  && (verifica_tablero_lleno(tablero) == 0)) {
			m_jugada = (m_jugada + 1) % COLUMNAS; 
		}
		
		//Se evalua cada columna
    	for (i = 0; i < COLUMNAS ; i++){
			
			//Se crea copia del tablero 
			//con el que se calcularan jugadas
	      	char copia[FILAS][COLUMNAS];
	      	copiar_matriz(tablero, copia);

			//Si la columa es valida se introduce la jugada en la copia
			//y se llama al minimax en modo minimizacion
	      	if(jugada_valida(i, copia)){
				introduce_jugada(copia, i, maximizacion);
				val = minimax(profundidad - 1, copia, false, alfa, beta);
				
				//Si el valor dado es mayor al mejor valor previo
				//el mejor balor ahora es valor
				//y la mejor jugada es la columna que lo produjo
		        if(val > m_valor){
		        	m_valor = val;
		        	m_jugada = i;
		        }
				
				//poda alfa bete
		        alfa = max(alfa, m_valor);
		        if (beta <= alfa){
		            break;
		        }
			}

      	}
		
	    //Jugada de la IA es la mejor jugada encontrada
		jugada_IA = m_jugada;
   
	//retornar mejor valor
    return m_valor;

 	} else {
		m_valor = 10000;
		
		//Inicializacion random
		m_jugada = rand() % COLUMNAS;
		//Si la jugada random no es valida se cambia hasta que lo sea
		while (!jugada_valida(m_jugada, tablero) && (verifica_tablero_lleno(tablero) == 0) ) {
			m_jugada = (m_jugada + 1)% COLUMNAS; 
		}
		
		//Se evalua cada columna
    	for (i = 0; i < COLUMNAS; i++){
			
			//Se crea copia del tablero con el que se calcularan jugadas
	      	char copia[FILAS][COLUMNAS];
	      	copiar_matriz(tablero, copia);
	
			
			//Si la columa es valida se introduce la jugada en la copia
			//y se llama al minimax en modo minimizacion
	      	if(jugada_valida(i, copia)){
				
				//Si el valor dado es mayor al mejor valor previo
				//el mejor balor ahora es valor
				//y la mejor jugada es la columna que lo produjo				
				introduce_jugada(copia, i, maximizacion);
				val = minimax(profundidad - 1, copia, true, alfa, beta);
				if (val < m_valor) {
					m_valor = min(val, m_valor);
					m_jugada = i;
				}

				//poda alfa beta
		        beta = min(beta, m_valor);
		        if (beta <= alfa){
		            break;
		        }
		    }
     
        } 
		
		
	  //Jugada de la IA es la mejor jugada encontrada
	  jugada_IA = m_jugada;
		
	  //Retornar mejor valoe	
      return m_valor;
   }
	
   jugada_IA = m_jugada;
}


//Funcion principal
int main(void) {	
	char tablero[FILAS][COLUMNAS], nombre[100];
	int col, opcion, sobrantes[COLUMNAS];
	int alfa = -1000000;
	int beta = 1000000;
	int ganador;
	
	
	//Se piden dimensiones del tablero
	while (1) {
		printf("Cuantas filas quieres en el tablero:\n");
		scanf("%d", &FILAS);
		if (FILAS > 3){
			break;
		}
		printf("Cantidad de filas debe de ser de almenos 4.\n");
	
	}
	
	while (1) {

		printf("Cuantas columnas quieres en el tablero:\n");
		scanf("%d", &COLUMNAS);
		
		if (COLUMNAS > 3) {
			break;
		}
		
		printf("Cantidad de columnas debe de ser de almenos 4.\n");
	}
		


	
	//Ciclo para repetir menu, opcion 3 sale del ciclo
	while (1) {
		ganador = 0;
		inicializacion(tablero);
		printf("Opciones\n");
		printf("1. Jugar contra la computadora\n");
		printf("2. Jugar humano vs humano\n");
		printf("3. Salir\n");
		scanf("%d", &opcion); 
		
		//opcion de jugar contra la computadora
		if (opcion == 1) {
			
			//Se piden datos del jugador humano
			printf("Deseas ser el jugador 1 o el jugador 2:\n");
			scanf("%d", &opcion);
			printf("Introduce el nombre del jugador:\n");
			scanf(" %[^\n]%*c", nombre);
			jugador[0].nombre = (char *)malloc(strlen(nombre) * sizeof(char));
			strcpy(jugador[0].nombre, nombre);

			//Si computadora es jugador 1 se marca columna central como su primera jugada
			if (opcion == 2) {
				printf("Computador\n");
				introduce_jugada(tablero, COLUMNAS >> 1, true);
		
			}
		
			printf("Opciones de colores\n");
			printf("1. Rojo\n");
			printf("2. Amarillo\n");
			scanf("%d", &jugador[0].color);
			if (jugador[0].color == 2) {
				jugador[1].color = 1;
			} else if (jugador[0].color == 1) {
				jugador[1].color = 2;
			}

			imprime_tablero(tablero);

			//Se inicia el juego
			while(!ganador){
		

				printf("Turno de %s (opciones desde la 1 hasta la %d)\n", jugador[0].nombre, COLUMNAS);
				scanf("%d", &col);

				
				//Si la jugada del humano es valida se introduce en el tablero
				if(((col>=1) && (col<=COLUMNAS)) && (jugada_valida(col-1, tablero))){
					
					introduce_jugada(tablero, col-1, false);
					
					//Si el humano no gano se llama a minimax
					if ((verifica_jugada_ganadora(tablero) != 1) && (verifica_tablero_lleno(tablero) == 0)) {
						minimax(PROFUNDIDAD, tablero, true, alfa, beta);
						introduce_jugada(tablero, jugada_IA, true);
					}

				}
				//Opcion invalida
				else{
					printf("Ocion invalida\n");
				}
			
				
				//Se imprime en caso de victoria de alguno de los jugadores o empate
				ganador = verifica_jugada_ganadora(tablero);
				if (ganador == 1) {
						printf("Gano el jugador: %s\n", jugador[0].nombre);
				} else if (ganador == 2) {
						printf("Gano el computador\n");
				} else if (verifica_tablero_lleno(tablero) == 1) {
					printf("Empate");
					imprime_tablero(tablero);
					break;
				}
		
				imprime_tablero(tablero);
			}	
		//Jugar humano vs humano
		} else if (opcion == 2) {
			
			//Se piden datos de los 2 jugadores
			printf("Introduce el nombre del jugador 1:\n");
			scanf(" %[^\n]%*c", nombre);
			jugador[0].nombre = (char *)malloc(strlen(nombre) * sizeof(char));
			strcpy(jugador[0].nombre, nombre);
			printf("Opciones de color\n");
			printf("1. Rojo\n");
			printf("2. Amarillo\n");
			printf("Que color quieres elegir:\n");
			scanf("%d", &jugador[0].color);
			if (jugador[0].color == 2) {
				jugador[1].color = 1;
			} else if (jugador[0].color == 1) {
				jugador[1].color = 2;
			}
			printf("Introduce el nombre del jugador 2:\n");
			scanf(" %[^\n]%*c", nombre);
			jugador[1].nombre = (char *)malloc(strlen(nombre) * sizeof(char));
			strcpy(jugador[1].nombre, nombre);

			//se inicia juego 
			while(!ganador){
		
				opcion = 1;
				imprime_tablero(tablero);

				//Se pide jugada del jugador 1
				while (1) {
					printf("Turno de %s (opciones desde la 1 hasta la %d)\n", jugador[0].nombre, COLUMNAS);
					scanf("%d", &col);

					//Si es valida se introduce, si es invalida se notifica y se pide denuevo
					if(((col>=1) && (col<=COLUMNAS)) && jugada_valida(col-1, tablero)){
						introduce_jugada(tablero, col-1, false);
						break;
					}
					else{
						printf("Opcion invalida\n");
					}			
				}
				imprime_tablero(tablero);
				
				//Si gano el jugador 1 se anuncia y se sale del juego
				if ((ganador = verifica_jugada_ganadora(tablero)) == 1) {

					printf("Gano el jugador: %s\n", jugador[0].nombre);

					break;
				//Si hay empate se notifica y se sale del juego
				} else if (verifica_tablero_lleno(tablero) == 1) {
					printf("Empate\n");
					imprime_tablero(tablero);
					break;
				}

				
				//Se piden opciones del jugador 2
				while (1) {
					printf("Turno de %s (opciones desde la 1 hasta la %d)\n", jugador[1].nombre, COLUMNAS);
					scanf("%d", &col);
		
					//Si la opcion es valida se introduce, sino se notifica y se vuelve a pedir
					if(((col>=1) && (col<=COLUMNAS)) && (jugada_valida(col-1, tablero))){
						introduce_jugada(tablero, col-1, true);
						break;
					}
					else{
						printf("Opcion invalida\n");
					}			
				}			
				
				//Si el jugador 2 gano se notifica y se sale del juego
				if ((ganador = verifica_jugada_ganadora(tablero)) == 2) {
	
					printf("Gano el jugador: %s\n", jugador[1].nombre);
				
				//Si hubo empate se notifica y se sale del juego
				} else if (verifica_tablero_lleno(tablero) == 1) {
					printf("Empate\n");
					imprime_tablero(tablero);
					break;
				}
				imprime_tablero(tablero);
			}	

		//Salir del menu
		} else if(opcion == 3) {
			break;
		//Opcion invalida
		} else {
			printf("Opcion invalida\n");
			
		}
	}
	
	//Si se asigno memoria dinamica a los nombres de usuario se libera
	if (strlen(jugador[0].nombre) > 0) {
		free(jugador[0].nombre);
	}

	if (strlen(jugador[1].nombre) > 0) {
		free(jugador[1].nombre);
	}
	
	return 0;
} 
