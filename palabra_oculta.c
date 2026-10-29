#include <stdio.h>
#include <string.h>


void palabraoculta(char palabra_secreta[], char palabra_descubrir[]){
    int largo_palabra_descubrir = strlen(palabra_secreta); 

    for (int i = 0; i < largo_palabra_descubrir  ; i++){
        palabra_descubrir [i] = '_'; 
    }
    palabra_descubrir [largo_palabra_descubrir] = '\0'; 


}

int main (){
    char palabra_secreta [1000] = "elefante";   
    char palabra_descubrir [1000] ; 

    palabraoculta(palabra_secreta, palabra_descubrir); 

    printf("%s", palabra_descubrir ); 

}