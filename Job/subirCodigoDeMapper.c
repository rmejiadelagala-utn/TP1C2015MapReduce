/*
 * subirCodigoDeMapper.c
 *
 *  Created on: 1/7/2015
 *      Author: rmejia
 */
#include "job.h"
#include <sys/stat.h>

char* subirCodigoFromPathToBuffer(char* path) {

	struct stat stat_file;
	stat(path, &stat_file);
	FILE* file = NULL;
	char* buffer = calloc(1, stat_file.st_size + 1);
	file=fopen(path,"r");
	if(file!=NULL){
		fread(buffer, stat_file.st_size, 1, file);
	} else {
		printf("no pude abrir archivo %s para enviar a Nodo\n",path);
		return NULL;
	}
	fclose(file);
	return buffer;
}
