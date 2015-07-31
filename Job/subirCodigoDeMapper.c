/*
 * subirCodigoDeMapper.c
 *
 *  Created on: 1/7/2015
 *      Author: rmejia
 */
#include "job.h"
#include <sys/stat.h>
#include <sys/mman.h>

char* subirCodigoFromPathToBuffer(char* path) {
	char* buffer;
	struct stat stat_file;
	stat(path, &stat_file);
	FILE* file = NULL;
	int size = stat_file.st_size;
	file=fopen(path,"r");
	if(file!=NULL){
		if ((buffer= mmap( NULL, size, PROT_READ, MAP_SHARED, fileno(file), 0))
				== MAP_FAILED) {
			//Si no se pudo ejecutar el MMAP, imprimir el error y abortar;
			printf("Error al ejecutar MMAP del archivo %s de tamaño: %d:\n",path, size);
			abort();
		}
	} else {
		printf("no pude abrir archivo %s para enviar a Nodo\n",path);
		return NULL;
	}
	fclose(file);
	return buffer;
}
