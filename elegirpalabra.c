
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "principal_struct.h"
#include "word_selection.h"

// Puntero a función para cargar un diccionario
typedef void (*DictionaryLoader)(Game *game);

// Carga una palabra aleatoria desde un archivo
void load_from_file(Game *game, const char *file_name) {
    FILE *file = fopen(file_name, "r");

    if (file == NULL) {
        printf("Error al abrir el archivo %s\n", file_name);
        return;
    }

    char word[MAX_WORD_LENGTH];
    int total_words = 0;

    while (fscanf(file, "%100s", word) == 1) {
        total_words++;
    }

    if (total_words == 0) {
        printf("El archivo %s esta vacio.\n", file_name);
        fclose(file);
        return;
    }

    int random_index = rand() % total_words;
    rewind(file);

    int counter = 0;

    while (fscanf(file, "%100s", word) == 1) {
        if (counter == random_index) {
            strcpy(game->secret_word, word);
            break;
        }
        counter++;
    }

    // Convertir la palabra a mayusculas
    for (int i = 0; game->secret_word[i] != '\0'; i++) {
        game->secret_word[i] = (char)toupper((unsigned char)game->secret_word[i]);
    }

    fclose(file);
}

void load_easy(Game *game) {
    load_from_file(game, "diccionario.txt");
}

void load_medium(Game *game) {
    load_from_file(game, "diccionariomedio.txt");
}

void load_difficult(Game *game) {
    load_from_file(game, "diccionariodifi.txt");
}

// Seleccion de palabra mediante Jump Table
void choose_word(Game *game, Difficulty difficulty) {
    DictionaryLoader loaders[] = {
        load_easy,
        load_medium,
        load_difficult
    };

    if (difficulty < EASY_MODE || difficulty > DIFFICULT_MODE) {
        difficulty = EASY_MODE;
    }

    loaders[difficulty](game);
}
