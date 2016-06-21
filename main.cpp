#include <stdio.h>
#include <conio.h>

const int M = 1000;

int menu();
void cesarEncriptarDesdeConsola(char texto[], char *p, int clave);
void cesarDesencriptarDesdeConsola(char texto[], char *p, int clave);
void cesarEncriptarDesdeArchivo(char texto[], char *p, int clave);
void cesarDesencriptarDesdeArchivo(char texto[], char *p, int clave);
int contarCaracteres(char texto[], char *p, char c);

int main() {
	//Código ASCII: 65 - 90 (A- Z) 97 - 122 (a - z)

	char texto[M];
	char *p = texto;

	// La clave es 0 por defecto por lo que el texto no se encriptará.
	int clave = 0;

	int opcionMenu = menu();

	while (opcionMenu != 0) {
		if (opcionMenu == 1) {
			printf("\nIntroduzca la nueva clave: ");
			scanf("%d", &clave);
			getchar();

			while (clave < 0 || clave > (char(90) - char(65))) {
				printf("ERROR: La clave no es valida \n\n");
				printf("Introduzca la nueva clave: ");
				scanf("%d", &clave);
				getchar();
			}

			printf("\n");
		}
		else {
			// Pregunta al usuario si el texto a desencriptar se tomará de un archivo o será introducido por consola
			int n;
			printf("1 - Encriptar/desencriptar desde un archivo de texto. \n");
			printf("2 - Encriptar/desencriptar un texto introducido por consola. \n\n");
			printf("Introduzca su opcion: ");
			scanf("%d", &n);
			getchar();

			if (opcionMenu == 2) {
				if (n == 1) {
					cesarEncriptarDesdeArchivo(texto, p, clave);
				}
				else if (n == 2) {
					printf("Introduzca el texto a encriptar: \n");
					fgets(texto, M, stdin);
					cesarEncriptarDesdeConsola(texto, p, clave);
				}
			}
			else if (opcionMenu == 3) {
				if (n == 1) {
					cesarDesencriptarDesdeArchivo(texto, p, clave);
				}
				else if (n == 2) {
					printf("Introduzca el texto a desencriptar: \n");
					fgets(texto, M, stdin);
					cesarDesencriptarDesdeConsola(texto, p, clave);
				}
			}
		}

		opcionMenu = menu();
	}

	printf("\nPulse ENTER para salir...");
	getchar();

	return 0;
}

int menu() {
	int opcion; 

	printf("1 - Cambiar la clave de encriptacion\n");
	printf("2 - Encriptar texto\n");
	printf("3 - Desencriptar texto\n");
	printf("0 - Salir\n\n");

	printf("Introduzca su opcion: ");
	scanf("%d", &opcion);
	getchar();

	while (opcion < 0 || opcion > 3) {
		printf("ERROR: La opción introducida no es valida\n\n");
		printf("Introduzca su opcion: ");
		scanf("%d", &opcion);
		getchar();
	}

	return opcion;
}

void cesarEncriptarDesdeConsola(char texto[], char *p, int clave) {
	int i = 0;

	while (*p != '\n') { 
		if (*p >= char(65) && *p <= char(90)) { // Letras mayúsculas [A-Z]
			texto[i] = char(char(65) + (char(*p) - char(65) + clave) % (char(90) - char(65) + 1));
		}
		else if (*p >= char(97) && *p <= char(122)) { // Letras minúsculas [a-z]
			texto[i] = char(char(97) + (char(*p) - char(97) + clave) % (char(122) - char(97) + 1));
		}

		printf("%c", texto[i]);
		i++;
		p++;
	}

	printf("\n\n");

}

void cesarDesencriptarDesdeConsola(char texto[], char *p,  int clave) {
	// Contamos los caracteres del texto encriptado introducido y los comparamos con un análisis de frequencia.

	//En el criptoanálisis, la técnica de análisis de frecuencias consiste en el aprovechamiento de estudios 
	// sobre la frecuencia de las letras o grupos de letras en los idiomas para poder establecer hipótesis para 
	// aprovecharlas para poder descifrar un texto cifrado sin tener la clave de descifrado.

	// Prueba a descifrarlo comparando la letras que más veces se repite en el texto con la letra más frecuente en castellano
	// y pregunta al usuario si el texto desencriptado tiene sentido. 
	// Si es así, entonces hemos encontrado la clave de desencriptación.
	// Si no es así, probamos a sustituir la letra más frecuente del texto con la segunda más frecuente del castellano, y así
	// sucesivamente.

	bool found = false;
	char frecuenciaCastellano[26] = {'e', 'a', 'o', 's', 'r', 'n', 'i', 'd', 'l', 'c', 't', 'u', 'm', 'p', 
		'b', 'g', 'v', 'y', 'q', 'h', 'f', 'z', 'j', 'x', 'w', 'k'};
	int frecuenciaTexto[26]; // Cada posición representa una letra del abecedario (0 - a, 1 - b, 2 - c, etc)

	// Inicializamos todas las letras a 0:
	for (int cont = 0; cont < 26; cont++) {
		frecuenciaTexto[cont] = 0;
	}

	char *puntero = p;
	while (*p != '\n') {
		if (*p >= char(65) && *p <= char(90)) { // Letras mayúsculas
			if (frecuenciaTexto[(int)*p - 65] == 0) {
				frecuenciaTexto[(int)*p - 65] += contarCaracteres(texto, p, *p);
			}
		}
		else if (*p >= char(97) && *p <= char(122)) { // Letras minúsculas
			if (frecuenciaTexto[(int)*p - 97] == 0) {
				frecuenciaTexto[(int)*p - 97] += contarCaracteres(texto, p, *p);
			}
		}
		p++;
	}

	// Comparamos las frecuencias obtenidas del texto introducido con las frecuencias de las letras en castellano:
	char maxCaracter = '\0';
	int maxFrecuencia = 0;
	for (int i = 0; i < 26; i++) {
		if (frecuenciaTexto[i] > maxFrecuencia) {
			maxFrecuencia = frecuenciaTexto[i];
			maxCaracter = char(i + 97);
		}
	}

	int j = 0;

	while (!found && j < 26) {
			int clave = (int)maxCaracter - (int)frecuenciaCastellano[j];

			if (clave < 0) {
				clave = 26 + clave;
			}

			cesarEncriptarDesdeConsola(texto, puntero, (char(90) - char(65) + 1) - clave);

			int opt;
			printf("El texto tiene sentido en castellano:\n");
			printf("1 - Si\n");
			printf("2 - No, seguir probando\n\n");
			printf("Introduzca su opcion: ");
			scanf("%d", &opt);
			getchar();

			while (opt < 1 || opt > 2) {
				printf("\nERROR: la opcion introducida no es valida.\n");
				printf("Introduzca su opcion: ");
				scanf("%d", &opt);
				getchar();
			}

			if (opt == 1) {
				found = true;
			}
			else {
				j++;
			}	
	}
}

void cesarEncriptarDesdeArchivo(char texto[], char *p, int clave) {
	FILE *pf; 
	pf = fopen("datos.txt", "r"); 
	
	if (pf == NULL) {
		printf("\nERROR: no se ha podido abrir el fichero.\n");
	}
	else {
		fgets(texto, M, pf);
		cesarEncriptarDesdeConsola(texto, p, clave);

		fclose(pf);
	}
}

void cesarDesencriptarDesdeArchivo(char texto[], char *p, int clave) {
	FILE *pf;
	pf = fopen("datos.txt", "r");

	if (pf == NULL) {
		printf("\nERROR: no se ha podido abrir el fichero.\n");
	}
	else {
		fgets(texto, M, pf);
		cesarDesencriptarDesdeConsola(texto, p, clave);

		fclose(pf);
	}
}

int contarCaracteres(char texto[], char *p, char c) {
	int n = 0;
	int i = 0;

	while (*p != '\n') {
		if (texto[i] == c) {
			n++;
		}

		i++;
		p++;
	}
	
	return n;
}