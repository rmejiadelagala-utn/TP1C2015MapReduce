/*
 * estructurasNodo.h
 *
 *  Created on: 8/7/2015
 *      Author: utnso
 */

#ifndef ESTRUCTURASNODO_H_
#define ESTRUCTURASNODO_H_

#include <stdint.h>

typedef struct {
	char* IP_FS;
	uint16_t PUERTO_FS;
	char* ARCH_BIN;
	char* DIR_TEMP;
	char NODO_NEW;
	char* IP_NODO;
	uint16_t PUERTO_NODO;
	int ID;
} t_config_nodo;

typedef struct {
	 int socket;
	char* ARCH_BIN;
	char* IP_NODO;
	uint16_t PUERTO_NODO;
	char NODO_NEW;
	char* DIR_TEMP;
	int ID;
}t_hilofs;

typedef struct {
	int tamanio;
	char* pathDestino;
	char* data;
}t_archivoTemporal;

typedef struct {
	int socket;
	char* registro;
}t_RegistroArch;

#endif /* ESTRUCTURASNODO_H_ */
