/*
 * nodo.c
 *
 *  Created on: 27/4/2015
 *      Author: utnso
 */
#include "nodo.h"

int main() {

	int i;
    char *DATOS, *DATOS_A;

	printf("Hola soy un proceso Nodo\n");

	char* path = "/home/utnso/ConfigNodo.txt";

	t_config* config;
	config = config_create(path);

	printf("Conectando a IP: %s\n", config_get_string_value(config, "IP_FS"));
	printf("Puerto: %d\n", config_get_int_value(config, "PUERTO_FS"));
	printf("Archivo_bin: %s\n", config_get_string_value(config, "ARCHIVO_BIN"));
	printf("Directorio Temporal: %s\n", config_get_string_value(config, "DIR_TEMP"));
	printf("Nodo Nuevo: %s\n", config_get_string_value(config, "NODO_NUEVO"));
	printf("IP Nodo: %s\n", config_get_string_value(config, "IP_NODO"));
	printf("Puerto Nodo: %d\n", config_get_int_value(config, "PUERTO_NODO"));

	DATOS = mapeo_disco("/home/utnso/midata1.bin");
	memset(DATOS + obtenerDirBloque(1),0, BLKSIZE );

	DATOS_A = mapeo_archivo("/home/utnso/Testeo.txt");
	memcpy(DATOS, DATOS_A + obtenerDirBloque(1), BLKSIZE);


	for(i=0; i<20; i++) {
		printf("%c", DATOS[i]);
	}

	int sockfs;
	sockfs = crearCliente(config_get_string_value(config,"IP_FS"),config_get_int_value(config, "PUERTO_FS"));


	//Probando un mensaje mandado del nodo al FS
	//send(sockfs,ip,1000 ,0);

	munmap(DATOS,sizeof(DATOS));
	return 0;

}

