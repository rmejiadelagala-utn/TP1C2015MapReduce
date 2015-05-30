/*
 * nodo.c
 *
 *  Created on: 27/4/2015
 *      Author: utnso
 */
#include "nodo.h"
#include "interfaces.h"

int main() {

	int i;
    char *DATOS, *DATOS_A;

	printf("Hola soy un proceso Nodo\n");

	char* path = "/home/utnso/ConfigNodo.txt";

	t_config_nodo* arch_config;

    arch_config= malloc(sizeof(t_config_nodo));

	arch_config = leerArchivoConfig(path);

	DATOS = mapeo_disco("/home/utnso/midata1.bin");
	memset(DATOS + obtenerDirBloque(1),0, BLKSIZE );

	DATOS_A = mapeo_archivo("/home/utnso/Testeo.txt");
	memcpy(DATOS, DATOS_A + obtenerDirBloque(1), BLKSIZE);


	for(i=0; i<20; i++) {
		printf("%c", DATOS[i]);
	}

	int sockfs;
	pthread_t hiloFS;
	pthread_t hiloJobsNodos;


	sockfs = crearCliente(arch_config->IP_FS,arch_config->PUERTO_FS);
	pthread_create(&hiloFS, NULL, (void*)interfazFS, (void*) &sockfs);
	pthread_create(&hiloJobsNodos, NULL, (void*)interfazJobNodos, (void*) &arch_config->PUERTO_NODO);


    pthread_join(hiloFS,NULL);
    pthread_join(hiloJobsNodos,NULL);

	//Probando un mensaje mandado del nodo al FS
	//send(sockfs,ip,1000 ,0);

	munmap(DATOS,sizeof(DATOS));
	return 0;

}

