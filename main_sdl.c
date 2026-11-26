#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "principal_struct.h"
#include "word_selection.h"
#include "hidden_word.h"
#include "f_agus.h"

TTF_Font *font_regular = NULL;
TTF_Font *font_title = NULL;

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 650

#define KEYBOARD_X 193
#define KEYBOARD_Y 390
#define KEY_WIDTH 50
#define KEY_HEIGHT 44
#define KEY_GAP 8
#define KEY_STEP 58

typedef enum {
    SCREEN_WELCOME,
    SCREEN_INSTRUCTIONS,
    SCREEN_MENU,
    SCREEN_GAME,
    SCREEN_RESULT
} Screen;


/* =========================
   FUNCIONES DE DIBUJADO
   ========================= */

void draw_text(SDL_Renderer *renderer, int x, int y,
               const char *text, Uint8 r, Uint8 g, Uint8 b) {

    TTF_Font *font = font_regular;

    if (font == NULL) return;

    SDL_Color color = {r, g, b, 255};

    SDL_Surface *surface = TTF_RenderText_Blended(
        font, text, 0, color
    );

    if (surface == NULL) return;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    if (texture != NULL) {
        SDL_FRect destination = {
            (float)x,
            (float)y,
            (float)surface->w,
            (float)surface->h
        };

        SDL_RenderTexture(renderer, texture, NULL, &destination);
        SDL_DestroyTexture(texture);
    }

    SDL_DestroySurface(surface);
}


void draw_text_centered(SDL_Renderer *renderer, int y,
                        const char *text, Uint8 r, Uint8 g, Uint8 b) {

    int text_width, text_height;

    if (font_regular == NULL) return;

    if (TTF_GetStringSize(font_regular, text, 0,
                          &text_width, &text_height)) {

        int x = (WINDOW_WIDTH - text_width) / 2;

        draw_text(renderer, x, y, text, r, g, b);
    }
}


void draw_title(SDL_Renderer *renderer, int x, int y,
                const char *text, Uint8 r, Uint8 g, Uint8 b) {

    if (font_title == NULL) return;

    SDL_Color color = {r, g, b, 255};

    SDL_Surface *surface = TTF_RenderText_Blended(
        font_title, text, 0, color
    );

    if (surface == NULL) return;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    if (texture != NULL) {
        SDL_FRect destination = {
            (float)x,
            (float)y,
            (float)surface->w,
            (float)surface->h
        };

        SDL_RenderTexture(renderer, texture, NULL, &destination);
        SDL_DestroyTexture(texture);
    }

    SDL_DestroySurface(surface);
}


void draw_button(SDL_Renderer *renderer, int x, int y,
                 int width, int height, const char *text,
                 Uint8 r, Uint8 g, Uint8 b) {

    SDL_FRect rect = {
        (float)x, (float)y,
        (float)width, (float)height
    };

    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderFillRect(renderer, &rect);

    int text_width, text_height;

    if (TTF_GetStringSize(font_regular, text, 0,
                          &text_width, &text_height)) {

        int text_x = x + (width - text_width) / 2;
        int text_y = y + (height - text_height) / 2;

        draw_text(renderer, text_x, text_y,
                  text, 255, 255, 255);
    }
}


bool inside_button(float mouse_x, float mouse_y,
                   int x, int y, int width, int height) {

    return mouse_x >= x && mouse_x <= x + width &&
           mouse_y >= y && mouse_y <= y + height;
}


/* =========================
   FUNCIONES DEL JUEGO
   ========================= */

void reset_game(Game *game) {

    memset(game->used_letters, false, sizeof(game->used_letters));

    game->remaining_attempts = 6;
    game->points = 6;
    game->status = STATUS_IN_GAME;

    initialize_hidden_word(game);
}


void start_game(Game *game, Difficulty difficulty) {

    char player_name[50];

    strcpy(player_name, game->player_name);

    memset(game, 0, sizeof(Game));

    strcpy(game->player_name, player_name);

    game->difficulty = difficulty;
    game->remaining_attempts = 6;
    game->points = 6;
    game->status = STATUS_IN_GAME;

    choose_word(game, difficulty);

    if (game->secret_word[0] != '\0') {
        initialize_hidden_word(game);
    }
}


/* =========================
   PANTALLA DE BIENVENIDA
   ========================= */

void draw_welcome(SDL_Renderer *renderer, Game *game) {

    draw_title(renderer, 365, 120, "AHORCADO", 255, 220, 100);

    draw_text_centered(renderer, 230,
                       "INGRESE SU NOMBRE:", 255, 255, 255);

    /* Recuadro para ingresar el nombre */
    SDL_FRect name_box = {200, 280, 500, 55};

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderRect(renderer, &name_box);

    /* Nombre y cursor centrados */
    int text_width = 0;
    int text_height = 0;
    int cursor_width = 0;
    int cursor_height = 0;

    if (font_regular != NULL) {

        TTF_GetStringSize(font_regular, game->player_name, 0,
                          &text_width, &text_height);

        TTF_GetStringSize(font_regular, "|", 0,
                          &cursor_width, &cursor_height);
    }

    int total_width = text_width + cursor_width + 2;
    int start_x = (WINDOW_WIDTH - total_width) / 2;

    if (game->player_name[0] != '\0') {

        draw_text(renderer, start_x, 295,
                  game->player_name, 100, 255, 150);
    }

    /* Cursor parpadeante */
    if ((SDL_GetTicks() / 500) % 2 == 0) {

        draw_text(renderer, start_x + text_width + 2, 295,
                  "|", 100, 255, 150);
    }

    draw_text_centered(renderer, 370,
                       "PRESIONE ENTER PARA CONTINUAR",
                       200, 200, 200);
}


/* =========================
   PANTALLA DE INSTRUCCIONES
   ========================= */

void draw_instructions(SDL_Renderer *renderer, Game *game) {

    char welcome_message[100];

    snprintf(welcome_message, sizeof(welcome_message),
             "¡BIENVENIDO/A %s!", game->player_name);

    /* Bienvenida con formato de título */
    int title_width = 0;
    int title_height = 0;

    if (TTF_GetStringSize(font_title, welcome_message, 0,
                          &title_width, &title_height)) {

        int title_x = (WINDOW_WIDTH - title_width) / 2;

        draw_title(renderer, title_x, 90,
                   welcome_message, 255, 220, 100);
    }

    /* Instrucciones con formato de texto centrado */
    draw_text_centered(renderer, 180,
                       "INSTRUCCIONES", 100, 255, 150);

    draw_text(renderer, 120, 250,
              "1. Seleccione una dificultad.", 255, 255, 255);

    draw_text(renderer, 120, 300,
              "2. Descubra la palabra ingresando letras.",
              255, 255, 255);

    draw_text(renderer, 120, 350,
              "3. Cada error reduce sus intentos y puntos.",
              255, 255, 255);

    draw_text(renderer, 120, 400,
              "4. Tiene 6 intentos para descubrir la palabra.",
              255, 255, 255);

    draw_text_centered(renderer, 510,
                       "PRESIONE ENTER PARA CONTINUAR",
                       200, 200, 200);
}


/* =========================
   MENU DE DIFICULTAD
   ========================= */

void draw_menu(SDL_Renderer *renderer) {

    draw_title(renderer, 365, 120,
               "AHORCADO", 255, 220, 100);

    draw_text(renderer, 335, 190,
              "SELECCIONE DIFICULTAD", 255, 255, 255);

    draw_button(renderer, 330, 250, 240, 55,
                "1 - FACIL", 40, 130, 100);

    draw_button(renderer, 330, 330, 240, 55,
                "2 - MEDIO", 40, 100, 160);

    draw_button(renderer, 330, 410, 240, 55,
                "3 - DIFICIL", 150, 70, 80);
}


/* =========================
   TECLADO DEL JUEGO
   ========================= */

void draw_keyboard(SDL_Renderer *renderer, Game *game) {

    for (int i = 0; i < 26; i++) {

        int row = i / 9;
        int column = i % 9;

        int x = KEYBOARD_X + column * KEY_STEP;
        int y = KEYBOARD_Y + row * (KEY_HEIGHT + 14);

        char letter[2] = {
            (char)('A' + i),
            '\0'
        };

        if (game->used_letters[i]) {

            draw_button(renderer, x, y, KEY_WIDTH, KEY_HEIGHT,
                        letter, 70, 70, 70);

        } else {

            draw_button(renderer, x, y, KEY_WIDTH, KEY_HEIGHT,
                        letter, 40, 110, 150);
        }
    }
}


/* =========================
   DIBUJO DEL AHORCADO
   ========================= */

void draw_hangman(SDL_Renderer *renderer, int remaining_attempts) {

    int errors = 6 - remaining_attempts;

    // Estructura de la horca
    SDL_SetRenderDrawColor(renderer, 150, 110, 70, 255);

    SDL_RenderLine(renderer, 650, 330, 820, 330);
    SDL_RenderLine(renderer, 690, 330, 690, 100);
    SDL_RenderLine(renderer, 690, 100, 790, 100);
    SDL_RenderLine(renderer, 790, 100, 790, 130);

    // Cabeza circular
    SDL_SetRenderDrawColor(renderer, 240, 220, 180, 255);

    if (errors >= 1) {

        SDL_RenderLine(renderer, 790, 130, 798, 132);
        SDL_RenderLine(renderer, 798, 132, 803, 137);
        SDL_RenderLine(renderer, 803, 137, 805, 145);
        SDL_RenderLine(renderer, 805, 145, 803, 153);
        SDL_RenderLine(renderer, 803, 153, 798, 158);
        SDL_RenderLine(renderer, 798, 158, 790, 160);
        SDL_RenderLine(renderer, 790, 160, 782, 158);
        SDL_RenderLine(renderer, 782, 158, 777, 153);
        SDL_RenderLine(renderer, 777, 153, 775, 145);
        SDL_RenderLine(renderer, 775, 145, 777, 137);
        SDL_RenderLine(renderer, 777, 137, 782, 132);
        SDL_RenderLine(renderer, 782, 132, 790, 130);
    }

    // Cuerpo
    if (errors >= 2) {
        SDL_RenderLine(renderer, 790, 160, 790, 235);
    }

    // Brazo izquierdo
    if (errors >= 3) {
        SDL_RenderLine(renderer, 790, 180, 755, 215);
    }

    // Brazo derecho
    if (errors >= 4) {
        SDL_RenderLine(renderer, 790, 180, 825, 215);
    }

    // Pierna izquierda
    if (errors >= 5) {
        SDL_RenderLine(renderer, 790, 235, 755, 280);
    }

    // Pierna derecha
    if (errors >= 6) {
        SDL_RenderLine(renderer, 790, 235, 825, 280);
    }
}


/* =========================
   PANTALLA DE JUEGO
   ========================= */

void draw_game(SDL_Renderer *renderer, Game *game) {

    char points_text[40];
    char attempts_text[40];

    draw_text(renderer, 390, 35,
              "AHORCADO", 255, 220, 100);

    draw_text(renderer, 100, 110,
              "PALABRA:", 255, 255, 255);

    draw_text(renderer, 100, 145,
              game->hidden_word, 100, 255, 150);

    snprintf(points_text, sizeof(points_text),
             "PUNTOS: %d", game->points);

    snprintf(attempts_text, sizeof(attempts_text),
             "INTENTOS: %d", game->remaining_attempts);

    draw_text(renderer, 100, 220,
              points_text, 255, 255, 255);

    draw_text(renderer, 100, 250,
              attempts_text, 255, 255, 255);

    draw_text(renderer, 100, 310,
              "SELECCIONE UNA LETRA:", 255, 255, 255);

    draw_keyboard(renderer, game);

    draw_hangman(renderer, game->remaining_attempts);
}


/* =========================
   DIBUJO DE ESTRELLAS
   ========================= */

void draw_star(SDL_Renderer *renderer, int center_x, int center_y,
               int radius, int fill_level) {

    const float PI = 3.14159265f;

    float x[10], y[10];
    float inner_radius = radius * 0.45f;

    // Crear los 10 vértices de la estrella
    for (int i = 0; i < 10; i++) {

        float angle = -PI / 2.0f + i * PI / 5.0f;
        float r = (i % 2 == 0) ? radius : inner_radius;

        x[i] = center_x + cosf(angle) * r;
        y[i] = center_y + sinf(angle) * r;
    }

    // Rellenar la estrella mediante líneas horizontales
    for (int scan = center_y - radius;
         scan <= center_y + radius; scan++) {

        float intersections[10];
        int count = 0;
        float scan_y = scan + 0.5f;

        for (int i = 0; i < 10; i++) {

            int next = (i + 1) % 10;

            if ((y[i] <= scan_y && y[next] > scan_y) ||
                (y[next] <= scan_y && y[i] > scan_y)) {

                float intersection = x[i] +
                    (scan_y - y[i]) * (x[next] - x[i]) /
                    (y[next] - y[i]);

                intersections[count++] = intersection;
            }
        }

        // Ordenar las intersecciones
        for (int i = 0; i < count - 1; i++) {

            for (int j = i + 1; j < count; j++) {

                if (intersections[i] > intersections[j]) {

                    float temp = intersections[i];
                    intersections[i] = intersections[j];
                    intersections[j] = temp;
                }
            }
        }

        // Dibujar los segmentos interiores
        for (int i = 0; i < count - 1; i += 2) {

            float left = intersections[i];
            float right = intersections[i + 1];

            if (fill_level == 2) {

                SDL_SetRenderDrawColor(renderer, 255, 210, 50, 255);
                SDL_RenderLine(renderer, left, scan, right, scan);

            } else if (fill_level == 1) {

                // Mitad izquierda dorada
                SDL_SetRenderDrawColor(renderer, 255, 210, 50, 255);

                if (left < center_x) {

                    float end = (right < center_x) ? right : center_x;
                    SDL_RenderLine(renderer, left, scan, end, scan);
                }

                // Mitad derecha oscura
                SDL_SetRenderDrawColor(renderer, 65, 65, 75, 255);

                if (right > center_x) {

                    float start = (left > center_x) ? left : center_x;
                    SDL_RenderLine(renderer, start, scan, right, scan);
                }

            } else {

                SDL_SetRenderDrawColor(renderer, 65, 65, 75, 255);
                SDL_RenderLine(renderer, left, scan, right, scan);
            }
        }
    }

    // Dibujar el contorno
    SDL_SetRenderDrawColor(renderer, 255, 220, 100, 255);

    for (int i = 0; i < 10; i++) {

        int next = (i + 1) % 10;

        SDL_RenderLine(renderer, x[i], y[i], x[next], y[next]);
    }
}


/* =========================
   PANTALLA DE RESULTADOS
   ========================= */


void draw_result(SDL_Renderer *renderer, Game *game, bool reveal_word) {

    char points_text[40];

    draw_text_centered(renderer, 90,
                       game->player_name, 255, 255, 255);

    if (game->status == STATUS_VICTORY) {
        draw_text_centered(renderer, 150,
                           "¡GANASTE!", 100, 255, 120);
    } else {
        draw_text_centered(renderer, 150,
                           "¡PERDISTE!", 255, 100, 100);
    }

    /* Mostrar la palabra solamente al ganar o al elegir NO */
    if (game->status == STATUS_VICTORY ||
    (game->status == STATUS_DEFEAT && reveal_word)) {

    draw_text_centered(renderer, 210,
                       "LA PALABRA ERA:", 255, 255, 255);

    draw_text_centered(renderer, 240,
                       game->secret_word, 255, 220, 100);
}

    float stars = calculate_stars(game);

    sprintf(points_text, "PUNTOS: %d", game->points);

    draw_text_centered(renderer, 290,
                       points_text, 255, 220, 100);

    for (int i = 0; i < 6; i++) {

    int fill_level = 0;

    if (stars >= i + 1) {
        fill_level = 2;
    } else if (stars >= i + 0.5f) {
        fill_level = 1;
    }

    draw_star(renderer, 325 + (i * 50), 370, 20, fill_level);
}

    /* Opciones al perder */
    if (game->status == STATUS_DEFEAT && !reveal_word) {
        draw_button(renderer, 220, 400, 200, 55,
                    "REINTENTAR", 40, 130, 100);

        draw_button(renderer, 480, 400, 200, 55,
                    "NO", 150, 70, 80);
    } else {
        draw_button(renderer, 350, 400, 200, 55,
                    "MENU PRINCIPAL", 60, 90, 150);
    }
}


/* =========================
   FUNCION PRINCIPAL
   ========================= */

int main(void) {

    Game game = {0};

    Screen screen = SCREEN_WELCOME;

    bool reveal_word = false;

    int name_length = 0;

    srand((unsigned int)time(NULL));

    if (!SDL_Init(SDL_INIT_VIDEO)) {

        printf("Error al iniciar SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "AHORCADO",
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0
    );

    if (window == NULL) {

        printf("Error al crear ventana: %s\n", SDL_GetError());

        SDL_Quit();
        return 1;
    }

    SDL_StartTextInput(window);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    if (renderer == NULL) {

        printf("Error al crear renderer: %s\n", SDL_GetError());

        SDL_StopTextInput(window);
        SDL_DestroyWindow(window);
        SDL_Quit();

        return 1;
    }

    if (!TTF_Init()) {

        printf("Error al iniciar SDL_ttf: %s\n", SDL_GetError());

        SDL_DestroyRenderer(renderer);
        SDL_StopTextInput(window);
        SDL_DestroyWindow(window);
        SDL_Quit();

        return 1;
    }

    font_regular = TTF_OpenFont("assets/fonts/segoeui.ttf", 22);
    font_title = TTF_OpenFont("assets/fonts/AGENCYB.TTF", 48);

    if (font_regular == NULL || font_title == NULL) {

        printf("Error al cargar fuentes: %s\n", SDL_GetError());

        if (font_regular != NULL) {
            TTF_CloseFont(font_regular);
        }

        if (font_title != NULL) {
            TTF_CloseFont(font_title);
        }

        TTF_Quit();

        SDL_DestroyRenderer(renderer);
        SDL_StopTextInput(window);
        SDL_DestroyWindow(window);
        SDL_Quit();

        return 1;
    }

    bool running = true;

    SDL_Event event;

    while (running) {

        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_EVENT_QUIT) {

                running = false;
            }

            // Borrar caracteres del nombre
            if (screen == SCREEN_WELCOME &&
                event.type == SDL_EVENT_KEY_DOWN &&
                event.key.key == SDLK_BACKSPACE) {

                if (name_length > 0) {

                    name_length--;
                    game.player_name[name_length] = '\0';
                }
            }

            // Avanzar desde bienvenida o instrucciones
            if (screen == SCREEN_WELCOME &&
                event.type == SDL_EVENT_KEY_DOWN &&
                event.key.key == SDLK_RETURN) {

                if (name_length > 0) {

                    screen = SCREEN_INSTRUCTIONS;
                }

            } else if (screen == SCREEN_INSTRUCTIONS &&
                       event.type == SDL_EVENT_KEY_DOWN &&
                       event.key.key == SDLK_RETURN) {

                screen = SCREEN_MENU;
            }

            // Capturar el nombre del jugador
            if (screen == SCREEN_WELCOME &&
                event.type == SDL_EVENT_TEXT_INPUT) {

                int length = strlen(event.text.text);

                if ((size_t)name_length + (size_t)length < sizeof(game.player_name)) {
                    strcat(game.player_name, event.text.text);
                    name_length += length;
                }
            }

            // Eventos del mouse
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN &&
                event.button.button == SDL_BUTTON_LEFT) {

                float mouse_x = event.button.x;
                float mouse_y = event.button.y;

                // Selección de dificultad
                if (screen == SCREEN_MENU) {

                    if (inside_button(mouse_x, mouse_y,
                                      330, 250, 240, 55)) {

                        start_game(&game, EASY_MODE);

                        screen = game.secret_word[0] != '\0'
                                 ? SCREEN_GAME : SCREEN_MENU;
                    }

                    if (inside_button(mouse_x, mouse_y,
                                      330, 330, 240, 55)) {

                        start_game(&game, MEDIUM_MODE);

                        screen = game.secret_word[0] != '\0'
                                 ? SCREEN_GAME : SCREEN_MENU;
                    }

                    if (inside_button(mouse_x, mouse_y,
                                      330, 410, 240, 55)) {

                        start_game(&game, DIFFICULT_MODE);

                        screen = game.secret_word[0] != '\0'
                                 ? SCREEN_GAME : SCREEN_MENU;
                    }
                }

                // Selección de letras
                else if (screen == SCREEN_GAME) {

                    for (int i = 0; i < 26; i++) {

                        int row = i / 9;
                        int column = i % 9;

                        int x = KEYBOARD_X + column * KEY_STEP;
                        int y = KEYBOARD_Y + row * (KEY_HEIGHT + 14);

                        if (inside_button(mouse_x, mouse_y,
                                          x, y, KEY_WIDTH, KEY_HEIGHT)) {

                            if (!game.used_letters[i]) {

                                game.used_letters[i] = true;

                                char letter = (char)('A' + i);

                                if (!check_letter(letter, &game)) {

                                    decrease_lives(&game);
                                }

                                if (strcmp(game.secret_word,
                                           game.hidden_word) == 0) {

                                    game.status = STATUS_VICTORY;
                                }

                                if (game.status != STATUS_IN_GAME) {
                                     /* Reiniciar la opción de revelar la palabra */
                                    reveal_word = false;
                                    
                                    if (game.status == STATUS_DEFEAT) {
                                        SDL_SetRenderDrawColor(renderer, 20, 25, 40, 255);
                                        SDL_RenderClear(renderer);
                                        draw_game(renderer, &game);
                                        SDL_RenderPresent(renderer);
                                        SDL_Delay(1500);


                                        /* Limpiar clics acumulados durante la pausa */
                                        SDL_Event discarded_event;
                                        while (SDL_PollEvent(&discarded_event)) {
                                            if (discarded_event.type == SDL_EVENT_QUIT) {
                                                running = false;
                                            }
                                        } 
                                    }
                                    
                                    screen = SCREEN_RESULT;
                                
                                }
                            }

                            break;
                        }
                    }
                }

                // Botones de resultados
                else if (screen == SCREEN_RESULT) {

                    if (game.status == STATUS_DEFEAT && !reveal_word) {

                        if (inside_button(mouse_x, mouse_y,
                                        220, 400, 200, 55)) {

                        reveal_word = false;
                        reset_game(&game);
                        screen = SCREEN_GAME;
                    }

                        else if (inside_button(mouse_x, mouse_y,
                                               480, 400, 200, 55)) {

                            reveal_word = true;
                        }
                    }

                    else if (inside_button(mouse_x, mouse_y,
                                           350, 400, 200, 55)) {

                        reveal_word = false;
                        screen = SCREEN_MENU;
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 20, 25, 40, 255);
        SDL_RenderClear(renderer);

        if (screen == SCREEN_WELCOME) {

            draw_welcome(renderer, &game);

        } else if (screen == SCREEN_INSTRUCTIONS) {

            draw_instructions(renderer, &game);

        } else if (screen == SCREEN_MENU) {

            draw_menu(renderer);

        } else if (screen == SCREEN_GAME) {

            draw_game(renderer, &game);

        } else {

            draw_result(renderer, &game, reveal_word);
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    // Liberar recursos
    SDL_StopTextInput(window);

    TTF_CloseFont(font_regular);
    TTF_CloseFont(font_title);

    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}