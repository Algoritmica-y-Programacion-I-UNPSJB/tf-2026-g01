#include <principal_struct.h>

float calculate_stars(Game *game) {
    if (game->status == STATUS_DEFEAT) {
        return 0.0f;
    }
    
    if (game->points <= 1) {
        return 1;
    }

    return (float)game->points;

}
