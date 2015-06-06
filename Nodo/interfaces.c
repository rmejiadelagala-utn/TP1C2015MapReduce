/*
 * interfaces.c
 *
 *  Created on: 25/5/2015
 *      Author: utnso
 */
#include "interfaces.h"
#include "nodo.h"

void* conexionFS(void* arg){

	printf("Conectado al Filesystem");

	t_hilofs* ptr;
	ptr = (t_hilofs*) arg;

	t_mensaje* mensaje = malloc(sizeof(t_mensaje));

	mensaje->id = CONEXION_FS;
	mensaje->tipo = 'N';

	strcpy(mensaje->info, ptr->IP_NODO);
	strcat(mensaje->info, ":");
	strcat(mensaje->info, string_itoa(ptr->PUERTO_NODO));

//	t_stream *stream = serializador(mensaje);
//	socket_enviar(socket, stream->data, stream->length);

	return 0;
}

void* conexionJobs(void * sockJOB){

	return 0;
}

void* conexionNodos(void * sockNodo){

	return 0;
}

