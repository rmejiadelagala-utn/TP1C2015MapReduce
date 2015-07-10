/*
 * fsystem.h
 *
 *  Created on: 10/5/2015
 *      Author: utnso
 */

#ifndef FSYSTEM_H_
#define FSYSTEM_H_

enum condConexionNodo {
	NODO_VIEJO_Y_NO_TENGO_SU_ID=0,
	NODO_NUEVO_Y_NO_TENGO_SU_ID=1,
	NODO_VIEJO_Y_TENGO_SU_ID=2,
	NODO_NUEVO_Y_TENGO_SU_ID=3,
};

//PROTOCOLOS QUE ESPERA EL FILESYSTEM




#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <socketes/servidor.h>
#include <socketes/estructuras.h>
#include <socketes/servidorMultihilo.h>
#include <socketes/envioDeMensajes.h>
#include <socketes/paquetesNodo.h>
#include <commons/log.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <commons/config.h>
#include "consolaFS.h"
#include "estructurasFileSystem.h"
#include <commons/collections/list.h>
#include "funcionesFileSystem.h"
#include "CUnit/Basic.h"
#include <unistd.h>
#include <semaphore.h>
#include <commons/string.h>
#include "interaccionMarta.h"
#include "persistencia.h"


t_config* config;

t_list *listaArchivos;
t_list *listaNodos;
t_list *listaDirectorios;
t_list *listaRegistrosIDIP;
sem_t semaforo;
sem_t consola_sem;
sem_t escuchar_sem;
FILE *archivoReconstruido;
int socketDeMarta;
FILE *fpArch;
FILE *fpDir;
FILE *fpNodos;



void levantarArchivoAMemoriaYDistribuirANodos(char* pathLocal, char* nombreArchivo, int padre);
#endif /* FSYSTEM_H_ */
