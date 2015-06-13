/*
 * paquetesNodo.c
 *
 *  Created on: 28/5/2015
 *      Author: Logonzalez
 */
#include "paquetesNodo.h"

t_stream * empaquetar_mensaje(t_mensaje *mensaje){

	char *data = malloc(strlen(mensaje->info) + 1 + sizeof(int) + sizeof(char));

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

t_mensaje * desempaquetar_mensaje(t_stream *stream) {

	t_mensaje *mensaje = malloc(sizeof(t_mensaje));

	int offset = 0;
	int tmp_size = 0;

	memcpy(&mensaje->id, stream->data, tmp_size = sizeof(char));
	offset = tmp_size;

	memcpy(&mensaje->tipo, stream->data + offset, tmp_size = sizeof(char));
	offset+= tmp_size;

	for (tmp_size = 1; (stream->data + offset)[tmp_size - 1] != '\0';tmp_size++);

	mensaje->info = malloc(tmp_size);
	memcpy(mensaje->info, stream->data + offset, tmp_size);

	return mensaje;
}

int enviar_mensaje(int unSocket, char* data, int length){

	int nbytes;
	if((nbytes=send(unSocket, data, length ,0)) < 0)
				perror("Error en el envio");

	return nbytes;
}

int recibir_mensaje (int unSocket, t_mensaje * mensaje){

	int nbytes;
	t_stream *stream = malloc(sizeof(t_stream));


	if ((nbytes= recv(unSocket, &(stream->length), sizeof(int), 0)) < 0)
		perror("Error en la recepcion");

	stream->data=malloc(stream->length);

	if ((nbytes= recv(unSocket, stream->data, stream->length, 0)) < 0)
			perror("Error en la recepcion");

	mensaje = desempaquetar_mensaje(stream);

	return nbytes;
}





