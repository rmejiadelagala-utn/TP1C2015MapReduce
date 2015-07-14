/*
 * MaRTA.h
 *
 *  Created on: 22/6/2015
 *      Author: utnso
 */

#ifndef MARTA_H_
#define MARTA_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/config.h>
#include<socketes/servidor.h>
#include <pthread.h>
#include "atencionJobs.h"
#include "interaccionMartaFS.h"
#include <socketes/envioDeMensajes.h>
#include <commons/collections/list.h>
#include <commons/string.h>
#include "tests.h"
#include "estructurasMaRTA.h"
#include <semaphore.h>
#include "interaccionJobs.h"

pthread_mutex_t mutexListaNodo;
t_list* cargaNodos;
t_list *listaRegistrosIDIP;
int socketDeFS; //socket cliente del FS
pthread_mutex_t contadorJobs;
pthread_mutex_t conexionFS;
pthread_mutex_t planificarMapMutex;
t_config *config;
sem_t funcionesMarta;
sem_t interaccionFS;
t_list* listaTemporal;

int contadorDeIdJob;

#endif /* MARTA_H_ */
