#include "tp0.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

const size_t CAP_INICIAL = 64;
const size_t LARGO_BUFFER = 128;
const int N_CAMPOS = 6;
const int SEP_CAMPOS = N_CAMPOS - 1;

char *leer_linea(FILE *archivo)
{
	if (!archivo)
		return NULL;
	size_t capacidad = CAP_INICIAL;
	size_t usados = 0;
	char *linea = malloc(capacidad);
	if (!linea)
		return NULL;

	char buffer[LARGO_BUFFER];
	int terminada = 0;

	while (!terminada && fgets(buffer, LARGO_BUFFER, archivo)) {
		size_t leidos = strlen(buffer);

		size_t requerido = usados + leidos + 1;
		if (capacidad < requerido) {
			size_t nueva = capacidad * 2;
			while (nueva < requerido)
				nueva *= 2;
			char *aux = realloc(linea, nueva);
			if (!aux) {
				free(linea);
				return NULL;
			}
			linea = aux;
			capacidad = nueva;
		}
		memcpy(linea + usados, buffer, leidos);
		usados += leidos;

		if (leidos > 0 && buffer[leidos - 1] == '\n') {
			terminada = 1;
		}
	}

	if (usados == 0) {
		free(linea);
		return NULL;
	}

	linea[usados] = '\0';

	if (usados > 0 && linea[usados - 1] == '\n') {
		linea[usados - 1] = '\0';
	}

	if (ferror(archivo)) {
		fprintf(stderr, "Error leyendo archivo: %s\n", strerror(errno));
		free(linea);
		return NULL;
	}
	return linea;
}

int mapear_tipo(const char *s, enum tipo_pokemon *out)
{
	struct {
		const char *txt;
		enum tipo_pokemon tipo;
	} tabla[] = { { "ELEC", TIPO_ELEC }, { "FUEG", TIPO_FUEG },
		      { "PLAN", TIPO_PLAN }, { "AGUA", TIPO_AGUA },
		      { "NORM", TIPO_NORM }, { "FANT", TIPO_FANT },
		      { "PSI", TIPO_PSI },   { "LUCH", TIPO_LUCH } };
	for (size_t i = 0; i < sizeof(tabla) / sizeof(tabla[0]); i++) {
		if (strcmp(s, tabla[i].txt) == 0) {
			*out = tabla[i].tipo;
			return 1;
		}
	}
	return 0;
}

int parse_int(const char *s, int *out)
{
	if (!s || *s == '\0')
		return 0;
	errno = 0;
	char *end;
	long v = strtol(s, &end, 10);
	if (end == s || *end != '\0' || errno == ERANGE || v < INT_MIN ||
	    v > INT_MAX)
		return 0;
	*out = (int)v;
	return 1;
}

void liberar_pokemon_parcial(struct pokemon *pk, char *copia)
{
	if (pk) {
		free(pk->nombre);
		free(pk);
	}
	free(copia);
}

int validar_ultimo_campo(char *p, char *campos[6])
{
	if (*p == '\0' || strchr(p, ',') != NULL)
		return 0;
	campos[5] = p;
	return 1;
}

struct pokemon *crear_pokemon(char *campos[6])
{
	struct pokemon *pk = malloc(sizeof *pk);
	if (!pk)
		return NULL;
	if (!parse_int(campos[0], &pk->id)) {
		free(pk);
		return NULL;
	}
	pk->nombre = malloc(strlen(campos[1]) + 1);
	if (!pk->nombre) {
		free(pk);
		return NULL;
	}
	strcpy(pk->nombre, campos[1]);
	if (!mapear_tipo(campos[2], &pk->tipo)) {
		free(pk->nombre);
		free(pk);
		return NULL;
	}
	if (!parse_int(campos[3], &pk->ataque) ||
	    !parse_int(campos[4], &pk->defensa) ||
	    !parse_int(campos[5], &pk->velocidad)) {
		free(pk->nombre);
		free(pk);
		return NULL;
	}
	return pk;
}

struct pokemon *parsear_pokemon(char *linea)
{
	if (!linea) {
		return NULL;
	}

	size_t len = strlen(linea) + 1;
	char *copia = malloc(len);
	if (!copia)
		return NULL;
	strcpy(copia, linea);

	char *campos[N_CAMPOS];
	int campo_idx = 0;
	char *p = copia;

	while (campo_idx < SEP_CAMPOS) {
		char *separacion = strchr(p, ',');
		if (!separacion) {
			free(copia);
			return NULL;
		}
		*separacion = '\0';

		if (*p == '\0') {
			free(copia);
			return NULL;
		}

		campos[campo_idx++] = p;
		p = separacion + 1;
	}

	if (*p == '\0') {
		free(copia);
		return NULL;
	}
	campos[5] = p;

	if (!validar_ultimo_campo(p, campos)) {
		free(copia);
		return NULL;
	}
	struct pokemon *pk = crear_pokemon(campos);
	if (!pk) {
		free(copia);
		return NULL;
	}
	free(copia);
	return pk;
}

void ordenar_pokemon(struct pokemon *pokemones, int cantidad_pokemones)
{
	if (!pokemones || cantidad_pokemones < 2) {
		return;
	}
	for (int i = 0; i < cantidad_pokemones - 1; i++) {
		for (int j = 0; j < cantidad_pokemones - 1 - i; j++) {
			if (strcmp(pokemones[j].nombre,
				   pokemones[j + 1].nombre) > 0) {
				struct pokemon tmp = pokemones[j];
				pokemones[j] = pokemones[j + 1];
				pokemones[j + 1] = tmp;
			}
		}
	}
}
