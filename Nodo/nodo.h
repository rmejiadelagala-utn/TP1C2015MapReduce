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
#include <commons/string.h>
#include "socketes/envioDeMensajes.h"
#include "socketes/servidor.h"
#include "socketes/servidorMultihilo.h"
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
#define BLKSIZE 50 //Ejemplo para probar el data.bin

typedef struct {
	char* IP_FS;
	uint16_t PUERTO_FS;
	char* ARCH_BIN;
	char* DIR_TEMP;
	char NODO_NEW;
	char* IP_NODO;
	uint16_t PUERTO_NODO;
} t_config_nodo;
typedef struct {
	 int socket;
	char* ARCH_BIN;
	char* IP_NODO;
	uint16_t PUERTO_NODO;
	char NODO_NEW;
	char* DIR_TEMP;
}t_hilofs;






#endif /* NODO_H_ */
