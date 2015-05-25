/*
 * nodoFunc.c
 *
 *  Created on: 18/5/2015
 *      Author: utnso
 */
#include "nodo.h"


char* mapeo_archivo(char* path){

	char* data_archivo;
	int fd_a = -1;
	uint32_t TAMANIOARCHIVO;

	if ((fd_a = open(path, O_RDONLY)) == -1)
		err(1, "Nodo: Error al abrir archivo (open)");

	struct stat bufa;

	stat(path, &bufa);
	TAMANIOARCHIVO = bufa.st_size;

	if ((data_archivo = mmap(0, TAMANIOARCHIVO, PROT_READ, MAP_SHARED, fd_a, 0)) == MAP_FAILED){
		close(fd_a);
		exit(1);
	}

	return data_archivo;
}

char* mapeo_disco(char* path){

	char* data_disco;
	int fd = -1;
	uint32_t TAMANIODISCO;

	if ((fd = open(path, O_RDWR)) == -1)
			err(1, "Nodo: Error al abrir midata1.bin (open)");

	struct stat buf;

	stat(path, &buf);

	TAMANIODISCO = buf.st_size;

	if ((data_disco = mmap(NULL, TAMANIODISCO, PROT_READ|PROT_WRITE, MAP_FILE|MAP_SHARED, fd, 0))== MAP_FAILED){;
		close(fd);
	    exit(1);
	}
	 return data_disco;
}


uint32_t obtenerDirBloque(uint32_t nroBloque){
	nroBloque = nroBloque * BLKSIZE;
	return nroBloque;
}






