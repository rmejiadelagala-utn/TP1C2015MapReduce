/*
 * funciones.h
 *
 *  Created on: 10/6/2015
 *      Author: utnso
 */

#ifndef FUNCIONES_H_
#define FUNCIONES_H_

#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <commons/string.h>
#include <sys/mman.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int obtener_cant_elem_array(char **);
char* obtenerNombreArchivo(char*);
char* mapeo_archivo(char*);

#endif /* FUNCIONES_H_ */
