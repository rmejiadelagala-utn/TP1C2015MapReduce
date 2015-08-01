/*
 * funcionesNodo.h
 *
 *  Created on: 7/6/2015
 *      Author: utnso
 */

#ifndef FUNCIONESNODO_H_
#define FUNCIONESNODO_H_

#define _FILE_OFFSET_BITS 64
#include "estructurasNodo.h"
#include "nodo.h"
#include <string.h>
#include <stdio.h>
#include <signal.h>
t_config_nodo* leerArchivoConfig(char *);
char* mapeo_archivo(char*);
char* mapeo_disco(char*);
uint32_t obtenerDirBloque(uint32_t);
uint32_t obtener_tamanio_disco(char*);
int obtener_cant_elem_array(char **);
char* obtenerNombreArchivo(char*);
char* getFileContent(char*, char *);
void crearScriptMapper(const char*,char*,int);
void crearScriptReduce(const char*,char*, int);
int ejecutarMapper(char*, char*, char*,int,uint32_t);
int ejecutarReduce(char*, char*, t_list*, int);
int redireccionar_stdin_stdout_mapper(char*,char*,char*,int,uint32_t);
int redireccionar_stdin_stdout_reduce(char*,char*,t_list*, int);//XXX ojota acá
char* aparear_registros(char*);
void guardarEnDisco(t_archivoTemporal* unArchivo);
void liberarArchivoTemporal(t_archivoTemporal* unArchivo);
t_RegistroArch* apareoDeRegistros(t_list*);
int aparear(t_list* archivosAReducir, int idReduce);

#endif /* FUNCIONESNODO_H_ */
