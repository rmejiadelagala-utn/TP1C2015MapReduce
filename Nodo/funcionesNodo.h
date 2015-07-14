/*
 * funcionesNodo.h
 *
 *  Created on: 7/6/2015
 *      Author: utnso
 */

#ifndef FUNCIONESNODO_H_
#define FUNCIONESNODO_H_

#include "estructurasNodo.h"
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
char* getFileContent(char*, char *);
void crearScriptMapper(const char*,char*);
void crearScriptReduce(const char*,char*);
void ejecutarMapper(char*, char*, char*);
void ejecutarReduce(char*, char*, char*);
int redireccionar_stdin_stdout_mapper(char*,char*,char*);
int redireccionar_stdin_stdout_reduce(char*,char*,char*);
char* aparear_registros(char*);
void guardarEnDisco(t_archivoTemporal* unArchivo);
void liberarArchivoTemporal(t_archivoTemporal* unArchivo);
t_RegistroArch* apareoDeRegistros(t_list*);

#endif /* FUNCIONESNODO_H_ */
