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
t_RegistroArch* apareoDeRegistros(t_list*);
//char* aparear_registros(char*);

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
/*	arch_mapeado = mapeo_archivo("/tmp/tempMapper1.txt");

	memcpy(copiar_datos, arch_mapeado, TAM_BLOQUE);
	copiar_datos = aparear_registros(copiar_datos);*/
	
	t_list* lista_reg = list_create();

	t_RegistroArch* data_reg = malloc(sizeof(t_RegistroArch));

	data_reg->registro = strdup("20130701;03011;7.;0;0015");
	data_reg->socket = 1;
	list_add(lista_reg, data_reg);

	data_reg = malloc(sizeof(t_RegistroArch));
	data_reg->registro = strdup("20130701;03011;2.0;0015");
	data_reg->socket = 2;
	list_add(lista_reg, data_reg);

	data_reg = malloc(sizeof(t_RegistroArch));
	data_reg->registro = strdup("20130701;03011;9.0;0015");
	data_reg->socket = 3;
	list_add(lista_reg, data_reg);

	void mostrarLista(t_RegistroArch* unReg) {
		printf("Registro:%s Socket:%d\n", unReg->registro,unReg->socket);
		fflush(stdout);
	}

	list_iterate(lista_reg, (void*) mostrarLista);

//	for(i=0; i< list_size(lista_reg); i++ ){
//		t_RegistroArch* unReg;
//		unReg = list_get(lista_reg, i);
//		printf("Registro lista:%s\n", unReg->registro);
//
//	}

	data_reg = apareoDeRegistros(lista_reg);
	printf("Registro:%s", data_reg->registro);
	strcpy(copiar_datos, data_reg->registro);

	ejecutarReduce("/tmp/reduce.pl", "/tmp/tempReduce1.txt", copiar_datos);
	free(copiar_datos);
	free(arch_mapeado);

	return 0;

}

int redireccionar_stdin_stdout(char *pathPrograma,char *pathArchivoSalida,char* data_bloque)
{
	FILE *stdin = NULL;

	char *comando = malloc(strlen(pathPrograma)+12+strlen(pathArchivoSalida));

	sprintf(comando,"%s | sort >> %s",pathPrograma, pathArchivoSalida);	//Ordena las cadenas entrantes

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

t_RegistroArch* apareoDeRegistros(t_list* Lista_reg){

	t_RegistroArch* datosReg = malloc(sizeof(t_RegistroArch));
	t_RegistroArch* infoRegArch;
	char* copiar_datos;

	copiar_datos = malloc(TAM_BLOQUE);

	infoRegArch = list_get(Lista_reg, 0);

	strcpy(copiar_datos, infoRegArch->registro);
	printf("La copia es %s\n", infoRegArch->registro);

	void realizoApareo(t_RegistroArch* unRegistro) {

		if ((strcmp(unRegistro->registro,copiar_datos)) <= 0){
			copiar_datos = malloc (TAM_BLOQUE);

			strcpy(copiar_datos, unRegistro->registro);
			datosReg->registro = strdup(copiar_datos);
//			strcat(datosReg->registro, "\n");
			datosReg->socket = unRegistro->socket;
		}

	}

	list_iterate(Lista_reg, (void*)realizoApareo);

	return datosReg;

}

/*char* aparear_registros(char* datos_archivo){

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
}*/
