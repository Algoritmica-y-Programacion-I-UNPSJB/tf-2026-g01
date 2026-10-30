#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


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

// 1. Definimos la firma genérica para la Jump Table
// "Puntero a una función que recibe la dirección de la Partida y no devuelve nada"
typedef void (*CargadorDiccionario)(Partida *partida);

void cargarDesdeArchivo(Partida *partida, const char *nombreArchivo) {
    FILE *archivo = fopen("diccionario.txt", "r");
    
    if (archivo == NULL) {
        printf("Error al abrir el archivo%s\n", nombreArchivo);
        return;
    }
    
    char palabraLectura[MAX_PALABRA];
    int totalPalabras = 0;

    while (fscanf(archivo, "%s", palabraLectura) == 1) {
        totalPalabras++;
    }
    if (totalPalabras == 0) {
        printf("El archivo %s está vacío.\n", nombreArchivo);
        fclose(archivo);
        return;
    }
    //elegimos una al azar
    int indiceAleatorio = rand() % totalPalabras;
    rewind(archivo); // Volvemos al inicio del archivo

    int contador = 0;
    while (fscanf(archivo, "%s", palabraLectura) == 1) {
        if (contador == indiceAleatorio) {
            strcpy(partida->palabraSecreta, palabraLectura);
            break;
        }
        contador++;
    }
    fclose(archivo);
}

void cargarFacil(Partida *partida){
    cargarDesdeArchivo(partida, "diccionario.txt");
}
void cargarMedio(Partida *partida){
    cargarDesdeArchivo(partida, "diccionariomedio.txt");
}
void cargarDificil(Partida *partida){
    cargarDesdeArchivo(partida, "diccionariodifi.txt");
}
//funcion principal que ejecuta el salto usando la jump table
void elegirPalabra(Partida *partida, int dificultad){
    //jump table de funciones que aceptan (partida*)
    CargadorDiccionario tablaCargadores[] = {
        NULL,   //posicion 0
        cargarFacil,   //posicion 1
        cargarMedio,   //posicion 2
        cargarDificil,   //posicion 3
    };

    if (dificultad < 1 || dificultad > 3) {
        dificultad = 1;  // Por defecto Fácil si se pasa un valor inválido
    }
    // ¡Salto directo O(1)! Llama a la función y le pasa la referencia del struct
    tablaCargadores[dificultad](partida);
}

int main() {
    srand(time(NULL)); // Se inicializa la semilla del azar al arrancar

    Partida miPartida;
    int dificultadSeleccionada = 2; // Supongamos que eligió Medio

    elegirPalabra(&miPartida, dificultadSeleccionada);

    printf("Palabra cargada en la estructura: %s\n", miPartida.palabraSecreta);

    // 2. TAREA DE LUDMILA: Usa juego.palabra_secreta para armar los "_ _ _ _"
    // inicializarPalabra(&juego);

    return 0;
}