/*
 * interfaces.h
 *
 *  Created on: 30/5/2015
 *      Author: utnso
 */

#ifndef INTERFACES_H_
#define INTERFACES_H_

#include "funcionesNodo.h"
#include "nodo.h"
#include <socketes/envioDeMensajes.h>

typedef struct {
	int ipNodo;
	int puertoNodo;
	char* nombreArch;
}t_archivoAReducir;

void* conexionFS(void*);
void* conexionJobs(void*);
t_archivoAReducir* nuevoArchReduce(int ip, int puerto, char* nombre);
//void* conexionNodos(void*);

#endif /* INTERFACES_H_ */
