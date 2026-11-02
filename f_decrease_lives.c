#include <stdio.h>
#include "principal_struct.h"

void decrease_lives (Game *game) {
	game->points--;
	game->remaining_attempts--;
	if (game->remaining_attempts == 0) {
        game->status = STATUS_DEFEAT;
    }
}