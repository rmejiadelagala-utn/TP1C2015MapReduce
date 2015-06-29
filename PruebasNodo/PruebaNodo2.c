#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <commons/string.h>
#include "socketes/funciones.h"

#define TAM_BLOQUE 1093

void ejecutarMapper(char*, char*, char*);
void ejecutarReduce(char*, char*, char*);
int redireccionar_stdin_stdout(char*,char*,char*);
char* aparear_registros(char*);

char* path_a = "PruebaMap3.txt";

int main (void) {

	char * copiar_datos;
	char * arch_mapeado;

	copiar_datos = malloc (TAM_BLOQUE);

	arch_mapeado = mapeo_archivo(path_a);

	memcpy(copiar_datos, arch_mapeado, TAM_BLOQUE);

//Pruebo Mapper de un bloque de Archivo
	ejecutarMapper("/tmp/mapper.sh", "/tmp/tempMapper1.txt", copiar_datos);
	free(copiar_datos);
	free(arch_mapeado);

//Pruebo Reduce de Registros apareados
	arch_mapeado = mapeo_archivo("/tmp/tempMapper1.txt");

	memcpy(copiar_datos, arch_mapeado, TAM_BLOQUE);
	copiar_datos = aparear_registros(copiar_datos);

	ejecutarReduce("/tmp/reduce.pl", "/tmp/tempReduce1.txt", copiar_datos);
	free(copiar_datos);
	free(arch_mapeado);

	return 0;

}

int redireccionar_stdin_stdout(char *pathPrograma,char *pathArchivoSalida,char* data_bloque)
{
	FILE *stdin = NULL;

	char *comando = malloc(strlen(pathPrograma)+11+strlen(pathArchivoSalida));

	sprintf(comando,"%s | sort > %s",pathPrograma, pathArchivoSalida);	//Ordena las cadenas entrantes

	stdin = popen (comando,"w");

	if (stdin != NULL){

		fprintf(stdin, "%s\n",data_bloque);

		pclose (stdin);
		free(comando);
	}
	else{
		printf("No se pudo ejecutar el programa!");
		return -1;
	}

	return 0;
}

void ejecutarMapper(char * path_s, char* path_tmp, char* datos_bloque){

	if ((redireccionar_stdin_stdout(path_s, path_tmp, datos_bloque)) < 0)
		printf("Error al ejecutar Mapper");

}

void ejecutarReduce(char * path_s, char* path_tmp, char* datos_bloque){

	if ((redireccionar_stdin_stdout(path_s, path_tmp, datos_bloque)) < 0)
		printf("Error al ejecutar Reduce");

}

char* aparear_registros(char* datos_archivo){

	char** reg_archivo;
	char* copiar_datos;
	int i;

	reg_archivo = string_split(datos_archivo, "\n");
	copiar_datos = malloc (TAM_BLOQUE);

	strcpy(copiar_datos, reg_archivo[0]);

	for(i = 0; reg_archivo[i]!= NULL; i++){

		if ((strcmp(reg_archivo[i],copiar_datos)) <= 0){
			copiar_datos = malloc (TAM_BLOQUE);
            		strcpy(copiar_datos, reg_archivo[i]);
		}

	}
    strcat(copiar_datos, "\n");
    return copiar_datos;
}
