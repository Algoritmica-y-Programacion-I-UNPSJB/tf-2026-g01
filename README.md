# Trabajo Integrador Final 2026 — Grupo 01

**Juego aprobado:** Ahorcado  
**Interfaz / Modalidad principal:** Gráfica (SDL3) — 1 jugador

## Integrantes y aportes

- Ludmila Aparicio (@lulyapa-git)
- Jonatan Parada (@YonitoP)
- Aylen Cardoso (@aylencc)
- Agustina Barrientos (@agustina-barrientos)

## Reglas y alcance

Juego del ahorcado para un jugador. El objetivo es descubrir una palabra secreta ingresando letras antes de quedarse sin intentos.

El juego cuenta con tres niveles de dificultad: fácil, medio y difícil. Las palabras se seleccionan aleatoriamente desde archivos de texto correspondientes a cada nivel.

Cada partida comienza con 6 intentos y 6 puntos. Por cada letra incorrecta se descuenta un intento y un punto. La puntuación final se expresa en estrellas y depende de la dificultad seleccionada.

El proyecto cuenta con dos versiones: una para consola y otra con interfaz gráfica desarrollada utilizando SDL3.

## Compilación y ejecución

### Requisitos

- Compilador GCC.
- SDL3.
- SDL3_ttf.
- pkg-config (necesario para obtener las opciones de compilación de SDL3).

La versión de consola no requiere bibliotecas externas adicionales.

### Versión consola

Desde la carpeta raíz del proyecto, ejecutar:

```bash
gcc -std=c11 -Wall -Wextra main.c elegirpalabra.c palabra_oculta.c f_check_letter.c f_decrease_lives.c f_stars_points.c -o ahorcado.exe
```

Para iniciar:

```bash
./ahorcado.exe
```

### Versión gráfica (SDL3)

Desde la carpeta raíz del proyecto, ejecutar:

```bash
gcc -std=c11 -Wall -Wextra main_sdl.c elegirpalabra.c palabra_oculta.c f_check_letter.c f_decrease_lives.c f_stars_points.c -o ahorcado_sdl.exe $(pkg-config --cflags --libs sdl3 sdl3-ttf) -lm
```

Para iniciar:

```bash
./ahorcado_sdl.exe
```

### Recursos necesarios

Para el correcto funcionamiento del juego, se deben conservar en la carpeta raíz:

- `diccionario.txt`
- `diccionariomedio.txt`
- `diccionariodifi.txt`
- La carpeta `assets/fonts/`, que contiene las fuentes utilizadas por la interfaz gráfica.

Los archivos de diccionario y las fuentes se cargan mediante rutas relativas a la carpeta raíz del proyecto.

## Organización de datos y funciones

### Estructuras principales

La estructura `Game`, definida en `principal_struct.h`, almacena los datos de la partida:

- Nombre del jugador.
- Palabra secreta y palabra oculta.
- Letras utilizadas.
- Intentos restantes y puntos.
- Estado de la partida.
- Nivel de dificultad.

Se utilizan los enumerados `Difficulty` y `GameStatus` para representar los niveles de dificultad y los estados del juego.

### Módulos principales

- `elegirpalabra.c`: selecciona una palabra aleatoria según la dificultad elegida, utilizando punteros a funciones.
- `palabra_oculta.c`: inicializa la palabra oculta.
- `f_check_letter.c`: comprueba si la letra ingresada pertenece a la palabra y revela sus apariciones.
- `f_decrease_lives.c`: descuenta intentos y puntos cuando se ingresa una letra incorrecta.
- `f_stars_points.c`: calcula la puntuación final expresada en estrellas.

Los encabezados correspondientes declaran las estructuras y funciones utilizadas por los distintos módulos.

## Interfaz gráfica

La versión gráfica utiliza SDL3 para crear la ventana, gestionar los eventos y renderizar los elementos del juego.

SDL3_ttf se utiliza para mostrar textos mediante fuentes externas.

La interfaz incluye:

- Pantalla de bienvenida e ingreso del nombre del jugador.
- Pantalla de instrucciones.
- Menú de selección de dificultad.
- Teclado gráfico para ingresar letras.
- Representación visual del ahorcado.
- Visualización de la palabra oculta, intentos y puntos.
- Pantalla de resultados con opción de reintentar o regresar al menú principal.