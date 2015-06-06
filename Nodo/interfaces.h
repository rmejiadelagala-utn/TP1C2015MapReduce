/*
 * interfaces.h
 *
 *  Created on: 30/5/2015
 *      Author: utnso
 */

#ifndef INTERFACES_H_
#define INTERFACES_H_

#define	CONEXION_FS 100 //Conexion con proceso FS: Envio IP y Puerto del Nodo

typedef struct{
	int  id;
	char tipo; //N: Nodo; M: Marta; F: Filesystem; J: Job
	char * info;
} __attribute__((__packed__)) t_mensaje;

typedef struct stream {
		int length;
		char* data;
	}__attribute__((__packed__)) t_stream;

#include <string.h>
#include <stdio.h>

void* conexionFS(void*);
void* conexionJobs(void*);
void* conexionNodos(void*);

#endif /* INTERFACES_H_ */
