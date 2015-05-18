/*
 * nodo.c
 *
 *  Created on: 27/4/2015
 *      Author: utnso
 */
#include<stdio.h>
#include<commons/config.h>
#include<socketes/servidor.h>

int main() {
	printf("Hola soy proceso Nodo\n");

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
	char* ip = config_get_string_value(config, "IP_NODO");
	char* archivo_bin = config_get_string_value(config, "ARCHIVO_BIN");

	int sockfs;
	sockfs = crearCliente(config_get_string_value(config,"IP_FS"),config_get_int_value(config, "PUERTO_FS"));

	//Probando un mensaje mandado del nodo al FS
	send(sockfs,ip,1000 ,0);
	return 0;

}

