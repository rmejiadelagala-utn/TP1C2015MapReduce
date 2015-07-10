/*
 * persistencia.h
 *
 *  Created on: 9/7/2015
 *      Author: utnso
 */

#ifndef PERSISTENCIA_H_
#define PERSISTENCIA_H_

#include "fsystem.h"


/**
 * @NAME: guardarBloqueEnNodo
 * @DESC: Guarda un bloqueEnNodo en el archivo apuntado por fpArch
 */
void guardarBloqueEnNodo(t_bloqueEnNodo *bloqueEnNodo) ;

/**
 * @NAME: cargarBloqueEnNodo
 * @DESC: carga un bloqueEnNodo desde el archivo apuntado por fpArch
 */
void cargarBloqueEnNodo();

/**
 * @NAME: guardarNodo
 * @DESC: Guarda un nodo en el archivo apuntado por fpNodos
 */
void guardarNodo(t_nodo *unNodo);

/**
 * @NAME: cargarNodo
 * @DESC: carga un nodo desde el archivo apuntado por fpNodos y lo agrega a la listaNodos
 */
t_nodo *cargarNodo();

/**
 * @NAME: guardarDirectorio
 * @DESC: Guarda un directorio en el archivo apuntado por fpDir
 */
void guardarDirectorio(t_directorio *unDir) ;

/**
 * @NAME: cargarDirectorio
 * @DESC: Carga un directorio desde el archivo apuntado por fpDir y lo agrega a la listaDirectorios
 */
t_directorio *cargarDirectorio();

void guardarListaDirectorios();
void cargarListaDirectorios();
void guardarListaNodos();
void cargarListaNodos();

#endif /* PERSISTENCIA_H_ */
