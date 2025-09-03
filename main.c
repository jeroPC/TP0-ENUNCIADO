#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "src/tp0.h"

const size_t MAIN_CAP_INICIAL = 4;
const size_t MAIN_FACTOR_CRE = 2;

void imprimir_pokemon(const struct pokemon *p)
{
	static const char *tipos[] = { "ELEC", "FUEG", "PLAN", "AGUA",
				       "NORM", "FANT", "PSI",  "LUCH" };
	printf("%d,%s,%s,%d,%d,%d\n", p->id, p->nombre, tipos[p->tipo],
	       p->ataque, p->defensa, p->velocidad);
}

bool agregar_pokemon(struct pokemon **vec, size_t *n, size_t *cap,
		     struct pokemon *pk)
{
	if (*n == *cap) {
		size_t nueva_cap = (*cap) * MAIN_FACTOR_CRE;
		struct pokemon *aux = realloc(*vec, nueva_cap * sizeof **vec);
		if (!aux) {
			perror("Error de memoria");
			return false;
		}
		*vec = aux;
		*cap = nueva_cap;
	}
	(*vec)[*n] = *pk;
	(*n)++;
	return true;
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "Uso: %s archivo.csv\n", argv[0]);
		return EXIT_FAILURE;
	}

	FILE *archivo = fopen(argv[1], "r");
	if (!archivo) {
		perror("Error al abrir archivo");
		return EXIT_FAILURE;
	}

	size_t capacidad = MAIN_CAP_INICIAL;
	struct pokemon *vector = malloc(capacidad * sizeof *vector);
	if (!vector) {
		perror("Error de memoria inicial");
		fclose(archivo);
		return EXIT_FAILURE;
	}
	size_t n = 0;
	char *linea;

	while ((linea = leer_linea(archivo))) {
		struct pokemon *pk = parsear_pokemon(linea);
		free(linea);
		if (!pk) {
			continue;
		}
		if (!agregar_pokemon(&vector, &n, &capacidad, pk)) {
			free(pk);
			break;
		}
		free(pk);
	}

	ordenar_pokemon(vector, (int)n);
	for (size_t i = 0; i < n; i++) {
		imprimir_pokemon(&vector[i]);
		free(vector[i].nombre);
	}

	free(vector);
	fclose(archivo);
	return EXIT_SUCCESS;
}