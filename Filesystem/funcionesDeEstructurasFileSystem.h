/*
 * funcionesDeEstructurasFileSystem.h
 *
 *  Created on: 31/5/2015
 *      Author: utnso
 */

#ifndef FUNCIONESDEESTRUCTURASFILESYSTEM_H_
#define FUNCIONESDEESTRUCTURASFILESYSTEM_H_

#include"estructurasFileSystem.h"

 //funciones para estructura Archivo
	/**
	* @NAME: nuevoArchivo
	* @DESC: Inicializa un nuevo archivo
	*/
 t_archivo nuevoArchivo(char nombre[255], int padre, float tamanio,
            t_bloqueArch *bloqueArch, int estado, int numeroDeBloqueEnArchivo);

	/**
	* @NAME: nuevoBloqueEnArchivo
	* @DESC: Inicializa un nuevo bloqueArchivo
	*/
t_bloqueArch nuevoBloqueEnArchivo(int numeroDeBloqueEnArchivo, t_bloqueEnNodo *copiaDeBloque);

	/**
	* @NAME: nuevoBloqueEnNodo
	* @DESC: Inicializa un nuevo bloqueNodo
	*/
t_bloqueEnNodo nuevoBloqueEnNodo(char ip[255], int puerto, int numeroDeBloqueEnNodo);


//funciones para la estructura nodo
	/**
	* @NAME: nuevoNodo
	* @DESC: Inicializa un nuevo nodo
	*/
t_nodo nuevoNodo(char ip[255], int puerto, int tamanio);

	/**
	* @NAME: activarNodo
	* @DESC: activa un nodo
	*/
void activarNodo(t_nodo unNodo);

	/**
	* @NAME: desactivarNodo
	* @DESC: desactiva un nodo
	*/
void desactivarNodo(t_nodo unNodo);

//funciones para la estructura tabla de directorios
	/**
	* @NAME: nuevoDirectorio
	* @DESC: crea una nueva entrada para el directorio
	*/
t_directorio nuevoDirectorio(int index, char nombre[255], int padre);


#endif /* FUNCIONESDEESTRUCTURASFILESYSTEM_H_ */
