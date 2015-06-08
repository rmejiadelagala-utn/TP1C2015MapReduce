/*
 * interfaces.c
 *
 *  Created on: 25/5/2015
 *      Author: utnso
 */
#include "interfaces.h"
#include "paquetesNodo.h"

void* conexionFS(void* arg){

    char *DATOS;
	uint32_t tam_disco;
	t_hilofs* ptr;
	ptr = (t_hilofs*) arg;

	printf("Nodo Conectado al Filesystem");

	tam_disco = obtener_tamanio_disco(ptr->ARCH_BIN);
	int cant_bloques = tam_disco / BLKSIZE;

	DATOS = mapeo_disco(ptr->ARCH_BIN);

	t_mensaje* mensaje = malloc(sizeof(t_mensaje));

	mensaje->id = CONEXION_FS;
	mensaje->tipo = 'N';

	//Formato de Mensaje-> IP:PUERTO:CANTBLOQUES
	string_append(&mensaje->info,ptr->IP_NODO);
	string_append(&mensaje->info, ":");
	string_append(&mensaje->info, string_itoa(ptr->PUERTO_NODO));
	string_append(&mensaje->info, string_itoa(cant_bloques));

/*	strcpy(mensaje->info, ptr->IP_NODO);
	strcat(mensaje->info, ":");
	strcat(mensaje->info, string_itoa(ptr->PUERTO_NODO));
	strcat(mensaje->info, string_itoa(cant_bloques));*/

	t_stream *stream = empaquetar_mensaje(mensaje);
	enviar_mensaje(ptr->socket, stream->data, stream->length);
	printf("Enviando mensaje a Filesystem\n");



	return 0;
}

void* conexionJobs(void * sockJOB){

	return 0;
}

void* conexionNodos(void * sockNodo){

	return 0;
}

