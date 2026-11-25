#include "principal_struct.h"

void decrease_lives(Game *game) {
    if (game->status != STATUS_IN_GAME) {
        return;
    }

	if (EASY_MODE) {
		game->points--;
		game->remaining_attempts--;
	}

	if (MEDIUM_MODE || DIFFICULT_MODE){
		game->points-2;
		game->remaining_attempts--;
	}

    if (game->remaining_attempts <= 0) {
        game->remaining_attempts = 0;
        game->status = STATUS_DEFEAT;
    }
}