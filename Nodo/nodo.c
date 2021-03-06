/*
 * nodo.c
 *
 *  Created on: 27/4/2015
 *      Author: utnso
 */
#include "nodo.h"

//Ruta donde se encuentra el archivo de configuración

int main(int argc,char *argv[]) {
	if(argc!=2){
			printf("Error: Necesito saber que numero de nodo soy, escribi:\n");
			printf("./Nodo ID, reemplazando ID por el numero de nodo\n");
			return 1;
		}
	char* c=argv[1];
	char* ARCHIVO_CONFIG = strdup("../Archivos/ConfigNodo");
	string_append(&ARCHIVO_CONFIG,c);
	string_append(&ARCHIVO_CONFIG,".txt");

	printf("***************HOLA SOY UN PROCESO NODO\n****************");

	pthread_mutex_init(&numeroMap,NULL);
	pthread_mutex_init(&numeroReduce,NULL);
	pthread_mutex_init(&unMutex,NULL);
	nodo_logger = log_create("nodo.log", "Nodo", 1, log_level_from_string("TRACE"));
	nodo_sync_logger = log_create("nodo_sync.log", "Nodo", 0, log_level_from_string("TRACE"));
	log_info(nodo_logger,"Cargando archivo de Configuracion");
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
		log_info(nodo_logger,"Nodo en la espera de conexiones de JOBs y otros Nodos.");

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

