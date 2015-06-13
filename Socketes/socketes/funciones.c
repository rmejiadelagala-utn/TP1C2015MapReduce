/*
 * funciones.c
 *
 *  Created on: 10/6/2015
 *      Author: utnso
 */
#include "funciones.h"

int obtener_cant_elem_array(char **array){

	int elem = 0;

	while(array[elem]!= NULL){
		elem++;
	}

  return elem;
}

char* obtenerNombreArchivo(char* ruta){

	char* file_name;
	char** array_path;
	int cant_elem;

	array_path = string_split(ruta, "/");
	cant_elem = obtener_cant_elem_array(array_path);
    file_name = array_path[cant_elem-1];

	return file_name;
}

char* mapeo_archivo(char* path){

	int fd_a = -1;

	char* data_archivo;
    uint32_t TAMANIOARCHIVO;

	if ((fd_a = open(path, O_RDONLY)) == -1)
		err(1, "FS: Error al abrir archivo (open)");

	struct stat bufa;

	stat(path, &bufa);
	TAMANIOARCHIVO = bufa.st_size;

	if ((data_archivo = mmap(0, TAMANIOARCHIVO, PROT_READ, MAP_SHARED, fd_a, 0)) == MAP_FAILED){
		printf("Error al iniciar el mapeo de Archivo %s. '%s' ", obtenerNombreArchivo(path), strerror(errno));
		close(fd_a);
		exit(1);
	}

	return data_archivo;
}
