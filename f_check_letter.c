
#include <stdbool.h>
#include <string.h>
#include "principal_struct.h"

// Verifica si la letra existe y revela todas sus apariciones
bool check_letter(char letter, Game *game) {
    bool found = false;
    int size = strlen(game->secret_word);

    for (int i = 0; i < size; i++) {
        if (letter == game->secret_word[i]) {
            game->hidden_word[i] = letter;
            found = true;
        }
    }

    return found;
}
