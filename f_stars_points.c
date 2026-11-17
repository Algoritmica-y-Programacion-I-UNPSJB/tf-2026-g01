#include "principal_struct.h"

// Calcula la puntuación final según la dificultad
float calculate_stars(Game *game) {
    if (game->status == STATUS_DEFEAT) {
        return 0.0f;
    }

    if (game->difficulty == EASY_MODE || game->difficulty == MEDIUM_MODE) {
        return (float)game->points;
    }

    if (game->difficulty == DIFFICULT_MODE) {
        return game->points / 2.0f;
    }

    return 0.0f;
}
