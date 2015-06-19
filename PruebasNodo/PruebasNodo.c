/*
 * PruebasNodo.c
 *
 *  Created on: 16/6/2015
 *      Author: Logonzalez
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define TAM_BLOQUE 200 //Ejemplo de bloque 200 bytes

int crearBloqueCompleto(char *,int);
void crearBloqueFinal(char *, int);

char* path_a = "PruebaMap.txt";


int main(void) {

	struct stat buff;
	int fd, tam_archivo;
	int offset = 0;
	int bytes_restantes = 0;
	int tam_final = 0;

	char * copiar_datos;
	void * arch_mapeado;

	copiar_datos = malloc (TAM_BLOQUE);

	/*obtener el descriptor del fichero*/
	if (( fd = open(path_a, O_RDONLY )) == -1){
		perror("Error al obtener el descriptor de fichero del archivo");
		exit(1);
	}

	stat(path_a, &buff);

	tam_archivo = buff.st_size;

	arch_mapeado = mmap(0, tam_archivo , PROT_READ, MAP_PRIVATE, fd, 0);

	if( arch_mapeado == MAP_FAILED){

		printf("Error al iniciar el mapeo de disco");
		close(fd);
		exit(1);
	}

	  for (;;){

		if( (tam_archivo - offset) < TAM_BLOQUE  ){

			tam_final = (tam_archivo - offset);

			memcpy(copiar_datos, arch_mapeado + offset, tam_final);

			crearBloqueFinal(copiar_datos, tam_final);

			free (copiar_datos);

			break;

		}

		memcpy(copiar_datos, arch_mapeado + offset, TAM_BLOQUE);

		bytes_restantes = crearBloqueCompleto(copiar_datos, TAM_BLOQUE);

		offset = offset + (TAM_BLOQUE - bytes_restantes);

	}

	munmap(arch_mapeado,buff.st_size);

	close(fd);

	return 0;
}

int crearBloqueCompleto(char *bloque_completo , int tam_bloque){

	int restante = tam_bloque - 1;

	while(restante >= 0 && bloque_completo[restante] != '\n')
	{
		bloque_completo[restante] ='0';

		restante--;

	}

	return TAM_BLOQUE - (restante + 1);
}

void crearBloqueFinal(char *bloque_final, int cant_escrita){

	int cont = TAM_BLOQUE - 1;

	while (cont >= cant_escrita ){

		bloque_final[cont] = '0';

		cont --;
	}
}
