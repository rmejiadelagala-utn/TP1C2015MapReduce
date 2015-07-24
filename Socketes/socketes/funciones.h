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
#include<commons/collections/list.h>

int obtener_cant_elem_array(char **);

char* obtenerNombreArchivo(char*);

char* mapeo_archivo(char*);

void lista_remove_and_destroy_by_condition(t_list *self,
bool (*condition)(void*), void (*element_destroyer)(void*)) ;
#endif /* FUNCIONES_H_ */
