/*
 * fsystem.h
 *
 *  Created on: 10/5/2015
 *      Author: utnso
 */

#ifndef FSYSTEM_H_
#define FSYSTEM_H_

#include <stdio.h>
#include <stdlib.h>
#include <socketes/servidor.h>
#include <socketes/estructuras.h>
#include <socketes/servidorMultihilo.h>
#include <socketes/paquetesNodo.h>
#include <commons/log.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <commons/config.h>
#include "consolaFS.h"
#include"estructurasFileSystem.h"
#include<commons/collections/list.h>
#include"funcionesFileSystem.h"
#include "CUnit/Basic.h"
#include <unistd.h>
#include <semaphore.h>

t_config* config;

t_list *listaArchivos;
t_list *listaNodos;
t_list *listaDirectorios;
sem_t semaforo;
FILE *archivoReconstruido;
int cantidadDeNodos;

#endif /* FSYSTEM_H_ */
