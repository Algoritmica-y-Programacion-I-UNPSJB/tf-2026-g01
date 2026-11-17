
#include <string.h>
#include "principal_struct.h"
#include "hidden_word.h"

// Inicializa la palabra oculta con guiones bajos
void initialize_hidden_word(Game *game) {
    int length = strlen(game->secret_word);

    for (int i = 0; i < length; i++) {
        game->hidden_word[i] = '_';
    }

    game->hidden_word[length] = '\0';
}
