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
#include <commons/collections/list.h>
#include <commons/string.h>
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
#include "interfaces.h"
#include "funcionesNodo.h"
#include "estructurasNodo.h"
#include <pthread.h>

//#define BLKSIZE 1024*1024*20
#define BLKSIZE 150 //Ejemplo para probar el data.bin



int numeroDeMap;
pthread_mutex_t numeroMap;
int numeroDeReduce;
pthread_mutex_t numeroReduce;




#endif /* NODO_H_ */
