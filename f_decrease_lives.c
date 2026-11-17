
#include "principal_struct.h"

// Descuenta un intento y un punto por una letra incorrecta
void decrease_lives(Game *game) {
    if (game->status != STATUS_IN_GAME) {
        return;
    }

    game->points--;
    game->remaining_attempts--;

    if (game->remaining_attempts <= 0) {
        game->remaining_attempts = 0;
        game->status = STATUS_DEFEAT;
    }
}
