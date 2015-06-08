/*
 * paquetesNodo.c
 *
 *  Created on: 28/5/2015
 *      Author: Logonzalez
 */
#include "paquetesNodo.h"

t_stream * empaquetar_mensaje(t_mensaje *mensaje){

	char *data = malloc(strlen(mensaje->info) + 1 + + sizeof(int)+ sizeof(char));

	t_stream *stream = malloc(sizeof(t_stream));
	int offset = 0, tmp_size = 0;

	memcpy(data, &mensaje->id, tmp_size = sizeof(int));
	offset = tmp_size;

	memcpy(data + offset, &mensaje->tipo, tmp_size = sizeof(char));
	offset += tmp_size;

	memcpy(data + offset, mensaje->info, tmp_size = strlen(mensaje->info) + 1);
	offset += tmp_size;

	stream->length = offset;
	stream->data = data;

	return stream;
}

int enviar_mensaje(int unSocket, char* data, int length){
	int nbytes;
	if((nbytes=send(unSocket, data, length ,0)) <0)
				perror("Error en el envio");
	return nbytes;
}

int recibir_mensaje(int unSocket, char* buffer){
	int nbytes;
	if ((nbytes = recv(unSocket, buffer, sizeof(buffer), 0)) < 0)
				perror("error en la recepcion");
	return nbytes;
}



