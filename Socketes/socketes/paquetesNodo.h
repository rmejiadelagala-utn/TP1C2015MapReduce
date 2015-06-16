/*
 * paquetesNodo.h
 *
 *  Created on: 28/5/2015
 *      Author: utnso
 */

#ifndef PAQUETESNODO_H_
#define PAQUETESNODO_H_

#include "servidor.h"
#define	CONEXION_FS     100 // Envio IP, Puerto y Cantidad de bloques total del Nodo al FS
#define	SET_BLOQUE      101 // Recibo del FS el bloque y datos a Grabar en el data.bin
#define	SET_BLOQUE_OK   103 // Envio OK de la escritura del bloque
#define GET_BLOQUE      104 // Recibo del FS el nro de Bloque
#define GET_BLOQUE_OK   105 // Envio el contenido del bloque solicitado por el FS
#define GET_FILE_       106 // Recibo del FS el nombre del archivo solicitado
#define GET_FILE_OK     107 // Envio el contenido del archivo solicitado por el FS

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
