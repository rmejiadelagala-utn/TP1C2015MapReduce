/*
 * funcionesNodo.h
 *
 *  Created on: 7/6/2015
 *      Author: utnso
 */

#ifndef FUNCIONESNODO_H_
#define FUNCIONESNODO_H_

#include "nodo.h"
#include <string.h>
#include <stdio.h>

t_config_nodo* leerArchivoConfig(char *);
char* mapeo_archivo(char*);
char* mapeo_disco(char*);
uint32_t obtenerDirBloque(uint32_t);
uint32_t obtener_tamanio_disco(char*);
int obtener_cant_elem_array(char **);
char* obtenerNombreArchivo(char*);

#endif /* FUNCIONESNODO_H_ */
