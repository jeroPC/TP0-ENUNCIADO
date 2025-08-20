#include <stdio.h>
#include <stdlib.h>
#include "src/tp0.h"

int main()
{
	char *nombre_archivo =
		"?????"; //Esto tiene que venir por línea de comando

	FILE *archivo = NULL; //¿Abrir archivo?

	char *linea = leer_linea(archivo); //¿Cómo hacemos?

	struct pokemon *pokemon = parsear_pokemon(linea);

	//Leer todas las líneas y luego mostrar todos los pokemon por pantalla en orden alfabético

	return 0;
}
