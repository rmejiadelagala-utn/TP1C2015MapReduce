/*
 * nodo.c
 *
 *  Created on: 27/4/2015
 *      Author: utnso
 */
#include "nodo.h"
#include "interfaces.h"

//Ruta donde se encuentra el archivo de configuración
char* ARCHIVO_CONFIG = "/home/utnso/ConfigNodo.txt";

int main() {

	int i;
    char *DATOS, *DATOS_A;

	printf("***************HOLA SOY UN PROCESO NODO\n****************");

	t_config_nodo* arch_config;

    arch_config= malloc(sizeof(t_config_nodo));
	arch_config = leerArchivoConfig(ARCHIVO_CONFIG);

/*	DATOS = mapeo_disco("/home/utnso/midata2.bin");
	memset(DATOS + obtenerDirBloque(1),0, BLKSIZE );

	DATOS_A = mapeo_archivo("/home/utnso/Testeo.txt");
	memcpy(DATOS, DATOS_A + obtenerDirBloque(1), BLKSIZE);


	for(i=0; i<20; i++) {
		printf("%c", DATOS[i]);
	}
	printf("\n");*/

	int sockfs;
	pthread_t hiloFS;
	pthread_t hiloJobs;
	pthread_t hiloNodos;
	t_hilofs paramHiloFS;

	//sockfs = 1;
	sockfs = crearCliente(arch_config->IP_FS,arch_config->PUERTO_FS);

	paramHiloFS.IP_NODO = arch_config->IP_NODO;
	paramHiloFS.PUERTO_NODO = arch_config->PUERTO_NODO;
	paramHiloFS.socket = sockfs;

	pthread_create(&hiloFS, NULL, (void*)conexionFS, (void*) &paramHiloFS);
	pthread_create(&hiloJobs, NULL, (void*)conexionJobs, (void*) &arch_config->PUERTO_NODO);
	pthread_create(&hiloNodos, NULL, (void*)conexionNodos, (void*) &arch_config->PUERTO_NODO);

    pthread_join(hiloFS,NULL);
    pthread_join(hiloJobs,NULL);
    pthread_join(hiloNodos,NULL);

	//Probando un mensaje mandado del nodo al FS
	//send(sockfs,ip,1000 ,0);

/*	munmap(DATOS,sizeof(DATOS));
	munmap(DATOS_A,sizeof(DATOS_A));

	DATOS = mapeo_disco("/home/utnso/midata2.bin");

	for(i=0; i<20; i++) {
		printf("%c", DATOS[i]);
	}
	printf("\n");
	munmap(DATOS,sizeof(DATOS));*/
	return 0;

}

