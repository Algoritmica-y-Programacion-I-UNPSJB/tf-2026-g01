#include "principal_struct.h"

// Calcula las estrellas en relación a los puntos
float calculate_stars(Game *game) {
    if (game->status == STATUS_DEFEAT) {
        return 0.0f;
    }
    
    //Puntuación minima en MEDIUM y DIFFICULT
    if (game->points <= 1) {
        return 1;
    }

    return (float)game->points;

}
