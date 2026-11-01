#include <stdio.h>

typedef enum buttons {
    start, // iniciar juego
    new_word, // renaudar juego
    finish, //finalizar juego 
}; buttons

typedef enum player_state { //formo un enum con el estado del jugador 
    not_started, //no empezo 
    playing, //jugando 
    finished, //finalizo 

}; player_state

void start_game (void){ //declaro una funcion para el inicio del juego 
    printf ("iniciando partida...\n");

}

void new_word_game (void){ //declaro una funcion para cuando el usuario solicite una nueva palabra 
    printf ("Seleccionando nueva palabra...\n");

}

void finish_game (void){ //declaro una funcion para cuando el juego finalice 
    printf ("Finalizando partida...\n"); 

}

void choose_button(buttons button, player_state *states ){ //declaro una funcion para el boton y estado 
    switch (button){ //hago un switch y en cada  case llamo a la funcion correspondiente 
        case start : 
        if (*states == not_started){ // antes de ir a la funcion verifico si el estado del player es no empezo a jugar 
        start_game();
        *states = playing; //luego el estado cambia a jugando
        }
        break;

        case new_word :
        if (*states == playing ){
        new_word_game();
        *states = playing; 
        }
        break; 

        case finish : 
        if (*states == playing){
        finish_game ();
        *states = finished; 
        }
        break; 

        default :
        printf ("Boton no valido.\n"); 
        break; 
    }


}

