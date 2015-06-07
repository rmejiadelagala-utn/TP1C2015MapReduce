/*
 * paquetesNodo.h
 *
 *  Created on: 28/5/2015
 *      Author: utnso
 */

#ifndef PAQUETESNODO_H_
#define PAQUETESNODO_H_

#define	CONEXION_FS 100 // Envio IP, Puerto y Cantidad de bloques total del Nodo al FS
#define	SET_BLOQUE  101 // Recibo del FS el bloque y datos a Grabar en el data.bin
#define GET_BLOQUE  102

#include "nodo.h"

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
int enviar_mensaje(int, char*, int);
int recibir_mensaje(int, char*);

#endif /* PAQUETESNODO_H_ */
