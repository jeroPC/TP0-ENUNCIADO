#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "src/tp0.h"
#include <errno.h>
#include <limits.h>

/* Prototipos para funciones POSIX usadas en tests */
char *strdup(const char *s);
FILE *fmemopen(void *buf, size_t size, const char *mode);

void test_parse_valida()
{
	const char *line = "1,Pikachu,ELEC,55,40,90";
	char *dup = strdup(line);
	struct pokemon *pk = parsear_pokemon(dup);
	assert(pk != NULL);
	assert(pk->id == 1);
	assert(strcmp(pk->nombre, "Pikachu") == 0);
	assert(pk->tipo == TIPO_ELEC);
	assert(pk->ataque == 55);
	assert(pk->defensa == 40);
	assert(pk->velocidad == 90);
	free(pk->nombre);
	free(pk);
	printf("test_parse_valida: OK\n");
}

void test_parse_invalida()
{
	const char *line = "2,Charmander,FUEG,52,43";
	char *dup = strdup(line);
	struct pokemon *pk = parsear_pokemon(dup);
	assert(pk == NULL);
	printf("test_parse_invalida: OK\n");
}

void test_ordenamiento()
{
	struct pokemon arr[3];

	arr[0].nombre = strdup("Zubat");
	arr[1].nombre = strdup("Abra");
	arr[2].nombre = strdup("Pikachu");
	arr[0].id = arr[1].id = arr[2].id = 0;
	arr[0].tipo = arr[1].tipo = arr[2].tipo = TIPO_PLAN;
	arr[0].ataque = arr[1].ataque = arr[2].ataque = 0;
	arr[0].defensa = arr[1].defensa = arr[2].defensa = 0;
	arr[0].velocidad = arr[1].velocidad = arr[2].velocidad = 0;

	ordenar_pokemon(arr, 3);
	assert(strcmp(arr[0].nombre, "Abra") == 0);
	assert(strcmp(arr[1].nombre, "Pikachu") == 0);
	assert(strcmp(arr[2].nombre, "Zubat") == 0);
	printf("test_ordenamiento: OK\n");
	free(arr[0].nombre);
	free(arr[1].nombre);
	free(arr[2].nombre);
}

void test_leer_simple()
{
	const char *data = "abc123\n";
	FILE *f = fmemopen((void *)data, strlen(data), "r");
	char *l = leer_linea(f);
	assert(l && strcmp(l, "abc123") == 0);
	free(l);
	fclose(f);
	printf("test_leer_simple: OK\n");
}

void test_leer_largo()
{
	size_t len = 5000;
	char *buf = malloc(len + 2);
	for (size_t i = 0; i < len; i++)
		buf[i] = 'x';
	buf[len] = '\n';
	buf[len + 1] = '\0';
	FILE *f = fmemopen(buf, len + 2, "r");
	char *l = leer_linea(f);
	assert(l && strlen(l) == len);
	free(l);
	fclose(f);
	free(buf);
	printf("test_leer_largo: OK\n");
}

void test_parse_mixto()
{
	const char *data = "1,Pika,ELEC,10,20,30\n"
			   "badline,nocomma\n"
			   "2,Char,FUEG,15,25,35\n";
	FILE *f = fmemopen((void *)data, strlen(data), "r");
	int count = 0;
	char *line;
	while ((line = leer_linea(f))) {
		struct pokemon *pk = parsear_pokemon(line);
		free(line);
		if (pk) {
			count++;
			free(pk->nombre);
			free(pk);
		}
	}
	fclose(f);
	assert(count == 2);
	printf("test_parse_mixto: OK\n");
}

int main(void)
{
	test_parse_valida();
	test_parse_invalida();
	test_ordenamiento();
	test_leer_simple();
	test_leer_largo();
	test_parse_mixto();
	printf("Todos los tests pasaron.\n");
	return EXIT_SUCCESS;
}
