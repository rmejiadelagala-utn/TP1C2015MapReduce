/*
 * nodo.c
 *
 *  Created on: 27/4/2015
 *      Author: utnso
 */
#include "nodo.h"

//Ruta donde se encuentra el archivo de configuración
char* ARCHIVO_CONFIG = "../ArchivosVarios/ConfigNodo.txt";

int main() {
	printf("***************HOLA SOY UN PROCESO NODO\n****************");

	t_config_nodo* arch_config;
	pthread_mutex_init(&numeroMap,NULL);
	pthread_mutex_init(&numeroReduce,NULL);

	printf("Cargando archivo de Configuracion\n");
    arch_config= malloc(sizeof(t_config_nodo));
	arch_config = leerArchivoConfig(ARCHIVO_CONFIG);

	int sockfs;
	pthread_t hiloFS;
//	pthread_t hiloJobs;
//	pthread_t hiloNodos;
	t_hilofs paramHiloFS;

	//sockfs = 1;
	sockfs = crearCliente(arch_config->IP_FS,arch_config->PUERTO_FS);

	paramHiloFS.socket = sockfs;
	paramHiloFS.IP_NODO = arch_config->IP_NODO;
	paramHiloFS.PUERTO_NODO = arch_config->PUERTO_NODO;
	paramHiloFS.ARCH_BIN = arch_config->ARCH_BIN;
	paramHiloFS.NODO_NEW = arch_config->NODO_NEW;
	paramHiloFS.DIR_TEMP = arch_config->DIR_TEMP;
	paramHiloFS.ID = arch_config->ID;

	pthread_create(&hiloFS, NULL, (void*)conexionFS, (void*) &paramHiloFS);
//	pthread_create(&hiloJobs, NULL, (void*)conexionJobs, (void*) &arch_config->PUERTO_NODO);
	if (crearServerMultiHilo(arch_config->PUERTO_NODO,(void*)conexionJobs) == 0)
		printf("Nodo en la espera de conexiones de JOBs y otros Nodos\n");

//	pthread_create(&hiloNodos, NULL, (void*)conexionNodos, (void*) &arch_config->PUERTO_NODO);

    pthread_join(hiloFS,NULL);
    //pthread_join(hiloJobs,NULL);
//    pthread_join(hiloNodos,NULL);

	//Probando un mensaje mandado del nodo al FS
	//send(sockfs,ip,1000 ,0);
	//printf("\n");
	//munmap(DATOS,sizeof(DATOS));
	return 0;

}

