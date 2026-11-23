
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

#include "principal_struct.h"
#include "word_selection.h"
#include "hidden_word.h"
#include "f_agus.h"

void reset_game(Game *game) {
    memset(game->used_letters, false, sizeof(game->used_letters));
    game->remaining_attempts = 6;
    game->points = 6;
    game->status = STATUS_IN_GAME;

    initialize_hidden_word(game);
}

bool ask_yes_no(const char *question) {
    char answer[10];

    do {
        printf("\n%s (SI/NO): ", question);

        if (scanf("%9s", answer) != 1) {
            return false;
        }

        for (int i = 0; answer[i] != '\0'; i++) {
            answer[i] = (char)toupper((unsigned char)answer[i]);
        }

        if (strcmp(answer, "SI") == 0) {
            return true;
        }

        if (strcmp(answer, "NO") == 0) {
            return false;
        }

        printf("Respuesta invalida. Escriba SI o NO.\n");

    } while (true);
}

Difficulty select_difficulty(void) {
    int option;

    do {
        printf("\n=== AHORCADO ===\n");
        printf("1. Facil\n");
        printf("2. Medio\n");
        printf("3. Dificil\n");
        printf("Seleccione el nivel de dificultad: ");
        if (scanf("%d", &option) != 1) {
            while (getchar() != '\n');
            option = 0;
        }

    } while (option < 1 || option > 3);

    return (Difficulty)(option - 1);
}

void play_game(Game *game) {
    char letter;
    int position;

    while (game->status == STATUS_IN_GAME) {
        printf("\nPalabra: ");
        for (int i = 0; game->hidden_word[i] != '\0'; i++) {
            printf("%c ", game->hidden_word[i]);
        }
        printf("\n");
        printf("Intentos restantes: %d\n", game->remaining_attempts);
        printf("Puntos: %d\n", game->points);
        printf("Ingrese una letra: ");

        if (scanf(" %c", &letter) != 1) {
            continue;
        }

        letter = (char)toupper((unsigned char)letter);

        if (letter < 'A' || letter > 'Z') {
            printf("Letra invalida. Ingrese una letra de la A a la Z.\n");
            continue;
        }

        position = letter - 'A';

        if (game->used_letters[position]) {
            printf("Ya utilizo esa letra.\n");
            continue;
        }

        game->used_letters[position] = true;

        if (check_letter(letter, game)) {
            printf("¡Letra correcta!\n");
        } else {
            printf("Letra incorrecta.\n");
            decrease_lives(game);
        }

        if (strcmp(game->secret_word, game->hidden_word) == 0) {
            game->status = STATUS_VICTORY;
        }
    }

    if (game->status == STATUS_VICTORY) {
        printf("\nPalabra: %s\n", game->secret_word);
        printf("¡Felicitaciones! ¡Ganaste!\n");
        printf("Estrellas obtenidas: %.1f\n", calculate_stars(game));
    } else {
        printf("\n¡Perdiste!\n");
        printf("Estrellas obtenidas: %.1f\n", calculate_stars(game));
    }
}

int main(void) {
    Game game;

    srand((unsigned int)time(NULL));

    do {
        memset(&game, 0, sizeof(Game));

        game.difficulty = select_difficulty();
        game.remaining_attempts = 6;
        game.points = 6;
        game.status = STATUS_IN_GAME;

        choose_word(&game, game.difficulty);

        if (game.secret_word[0] == '\0') {
            printf("No se pudo cargar una palabra. Finalizando el juego.\n");
            return 1;
        }

        initialize_hidden_word(&game);

        do {
            play_game(&game);

            if (game.status == STATUS_DEFEAT) {

                if (ask_yes_no("¿Desea volver a intentarlo con la misma palabra?")) {
                    reset_game(&game);
                } else {
                    printf("\nLa palabra era: %s\n", game.secret_word);
                }

            } else {
                break;
            }

        } while (game.status == STATUS_IN_GAME);

        if (game.status == STATUS_DEFEAT) {
            break;
        }

    } while (ask_yes_no("¿Desea jugar otra partida?"));

    printf("\n¡Gracias por jugar!\n");

    return 0;
}
