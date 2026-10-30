#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CANT_PALABRAS 5
#define MAX_PALABRA 30

typedef enum{
    ESTADO_JUGANDO,
    ESTADO_VICTORIA,
    ESTADO_DERROTA,
} EstadoPartida;

typedef struct{
    char palabraSecreta[MAX_PALABRA]; // palabra a adivinar
    char palabraOculta[MAX_PALABRA];  // los guiones bajos
    int vidasRestantes; //vidas (son 6)
    bool letrasUsadas[26]; //registro de letras usadas
    EstadoPartida estado; //estado de la ronda
    int estrellitas; //cantidad de estrellas
} Partida;

 //diccionario de palabras hardcodeado 
const char *diccionario[CANT_PALABRAS] = {
    "PERRO",
    "GATO",
    "CASA",
    "COMPUTADORA",
    "TECLADO"
};

void elegirPalabra(Partida *partida) {
    FILE *archivo = fopen("diccionario.txt", "r");
    
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }
    // 1. Elegimos un índice aleatorio en el rango del diccionario
    int indiceAzar = rand() % CANT_PALABRAS;

    // 2. Copiamos la palabra elegida a la estructura del juego
    strcpy(partida->palabraSecreta, diccionario[indiceAzar]);
}

int main() {
    srand(time(NULL)); // Se inicializa la semilla del azar al arrancar

    Partida miPartida;

    // 1. TU TAREA (Aylen): Elige la palabra y la guarda en juego.palabra_secreta
    elegirPalabra(&miPartida);

    // 2. TAREA DE LUDMILA: Usa juego.palabra_secreta para armar los "_ _ _ _"
    // inicializarPalabra(&juego);

    return 0;
}