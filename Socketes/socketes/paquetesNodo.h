/*
 * paquetesNodo.h
 *
 *  Created on: 28/5/2015
 *      Author: utnso
 */

#ifndef PAQUETESNODO_H_
#define PAQUETESNODO_H_

#include "servidor.h"

typedef struct{
	int  id;
	char tipo; //N: Nodo; M: Marta; F: Filesystem; J: Job
	char * info;
} __attribute__((__packed__)) t_mensaje;

typedef struct stream {
		int length;
		char* data;
	}__attribute__((__packed__)) t_stream;

t_stream * empaquetar_mensaje(t_mensaje *);
t_mensaje * desempaquetar_mensaje(t_stream *);
int enviar_mensaje(int, char*, int);
int recibir_mensaje (int, t_mensaje **);

#endif /* PAQUETESNODO_H_ */
