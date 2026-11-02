#include <stdio.h>
#include <principal_struct.h>

float stars_points (Game *game) {
    //Definición de puntos según casos:
	if (game->status == STATUS_DEFEAT) {
        return 0;
    }

    if (game->dificulty == EASY_MODE || game->dificulty == MEDIUM_MODE) {
        return game->points;
    } else if (game->dificulty == DIFFICULT_MODE) {
        return game->points/2;
    }
}