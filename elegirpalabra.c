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

// TODO: evaluar migrar 'palabra secreta' y 'palabra oculta' a memoria dinamica para evitar desperdicio de RAM
typedef struct{
    char palabraSecreta[MAX_PALABRA]; // palabra a adivinar 
    char palabraOculta[MAX_PALABRA];  // los guiones bajos 
    int vidasRestantes; //vidas (son 6)
    bool letrasUsadas[26]; //registro de letras usadas   // chequear esto (??? parte de agus
    EstadoPartida estado; //estado de la ronda
    int estrellitas; //cantidad de estrellas
} Partida;

// 1. Definimos la firma genérica para la Jump Table
// "Puntero a una función que recibe la dirección de la Partida y no devuelve nada"

// TODO: Revisar la firma genérica cuando las chicas integren el manejo de banderas/letras usadas.
typedef void (*CargadorDiccionario)(Partida *partida);

void cargarDesdeArchivo(Partida *partida, const char *nombreArchivo) {  
    FILE *archivo = fopen("diccionario.txt", "r");  // chequear esto (???
    
    if (archivo == NULL) {
      // FIXME: Si en el futuro 'partida' o 'dic' usan memoria dinámica (malloc),
      // acordarse de hacer free(...) antes de hacer el return para no dejar memory leaks.
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
        NULL,            //posicion 0 Nula (no existe dificultad 0) posicion "relleno"
        cargarFacil,     //posicion 1  Apunta a cargar_facil
        cargarMedio,     //posicion 2  Apunta a cargar_medio
        cargarDificil,   //posicion 3  Apunta a cargar_dificil
    };

    if (dificultad < 1 || dificultad > 3) { //fallback 
        dificultad = 1;  // Por defecto Fácil si se pasa un valor inválido
    }
    // Salto directo O(1) Llama a la función y le pasa la referencia del struct
    tablaCargadores[dificultad](partida);
}

int main() {
    srand(time(NULL)); // Se inicializa la semilla del azar al arrancar
    Partida miPartida;

    //Hardcodeo temporal para probar la Jump Table sin interfaz de usuario
    // TODO: Reemplazar por la lectura real de la opción elegida por el usuario
    int dificultadSeleccionada = 2; 
    //Prueba de invocación O(1) con dificultad 'Medio'
    elegirPalabra(&miPartida, dificultadSeleccionada);

    //Verificación de que la palabra se cargó correctamente en el struct
    printf("Palabra cargada en la estructura: %s\n", miPartida.palabraSecreta);



    return 0;
}