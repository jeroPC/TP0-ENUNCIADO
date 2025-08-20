#ifndef TP0_H_
#define TP0_H_
#include <stdio.h>

struct pokemon {
	int id;
	char *nombre;
	int ataque;
	int defensa;
	int velocidad;
};

/**
 * Lee una línea entera de un archivo abierto. Devuelve la línea leída o NULL en
 * caso de error.
 */
char *leer_linea(FILE *archivo);

/**
 * Dado un string en formato CSV (mirar enunciado), crea un pokemon con los
 * datos del mismo. Devuelve el pokemon creado o NULL en caso de error.
 */
struct pokemon *parsear_pokemon(char *linea);

/**
 * Dado un vector de cantidad_pokemones, ordena el vector por nombre.
 */
void ordenar_pokemon(struct pokemon *pokemones, int cantidad_pokemones);

#endif // TP0_H_
