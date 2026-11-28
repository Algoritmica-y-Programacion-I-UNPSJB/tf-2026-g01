
#include "principal_struct.h"

// EASY_MODE Descuenta un intento y un punto por una letra incorrecta
// MEDIUM/DIFFICULT MODE Descuenta un intento y dos puntos por letra incorrecta
void decrease_lives(Game *game) {
    if (game->status != STATUS_IN_GAME) {
        return;
    }

    if (game->difficulty == EASY_MODE) {
		game->points--;
		game->remaining_attempts--;
	}

	if (game->difficulty == MEDIUM_MODE || game->difficulty == DIFFICULT_MODE){
		game->points -= 2;
		game->remaining_attempts--;
	}

    if (game->remaining_attempts <= 0) {
        game->remaining_attempts = 0;
        game->status = STATUS_DEFEAT;
    }
}
