
#ifndef PRINCIPAL_STRUCT_H
#define PRINCIPAL_STRUCT_H

#include <stdbool.h>

#define MAX_WORD_LENGTH 101

typedef enum {
    EASY_MODE,
    MEDIUM_MODE,
    DIFFICULT_MODE
} Difficulty;

typedef enum {
    STATUS_IN_GAME,
    STATUS_VICTORY,
    STATUS_DEFEAT
} GameStatus;

typedef struct {
    char player_name[50];
    char secret_word[MAX_WORD_LENGTH];
    char hidden_word[MAX_WORD_LENGTH];
    bool used_letters[26];
    int remaining_attempts;
    int points;
    GameStatus status;
    Difficulty difficulty;
} Game;

#endif
