#ifndef principal_struct_h
#define principal_struct_h

#include <stdio.h>
#define max 101
///* Modelo Struct
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
    char secret_word[max];
    char hidden_word[max];
    char letter_history[max];// Historial de letras probadas
    int letter_history_counter;
    int remaining_attempts;
    int points;
    GameStatus status;
    Difficulty dificulty;
} Game;

#endif

//Game game;
//*/