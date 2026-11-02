#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "principal_struct.h"

///* Función de verificación de letra
bool check_letter (char letter, Game *game) {
    bool found = false;
    int size = strlen(game->secret_word);
    //Se recorre el arreglo hidden_word por completo para modificar todas las letras posibles
    for (int i = 0;i < size;i++) {
        if (letter == game->hidden_word[i]) {
        game->secret_word[i] = letter;
        found = true;
        }
    }
    
    if (found) {
        return true;
    } else {
        return false;
    }
}
//*/