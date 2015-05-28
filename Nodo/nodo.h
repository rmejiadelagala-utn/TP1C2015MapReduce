/*
 * nodo.h
 *
 *  Created on: 25/5/2015
 *      Author: utnso
 */

#ifndef NODO_H_
#define NODO_H_

#include <stdio.h>
#include <commons/config.h>
#include <socketes/servidor.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include <errno.h>
#include <fcntl.h>
#include <err.h>
#include <time.h>
#include <pthread.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/stat.h>

//#define BLKSIZE 1024*1024*20
#define BLKSIZE 10 //Ejemplo para probar el data.bin

uint32_t TAMANIODISCO, TAMANIOARCHIVO;

char* mapeo_archivo(char*);
char* mapeo_disco(char*);
uint32_t obtenerDirBloque(uint32_t);

#endif /* NODO_H_ */