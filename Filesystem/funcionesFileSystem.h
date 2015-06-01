/*
 * funcionesFileSystem.h
 *
 *  Created on: 31/5/2015
 *      Author: JuanRios
 */

#ifndef FUNCIONESFILESYSTEM_H_
#define FUNCIONESFILESYSTEM_H_

	/*
  	* @NAME: distribuirBloquesEnNodos
  	* @DESC: recibe una lista de bloques de archivo, por la cual crea 3 copias y cada una se la asigna a un nodo específico. Ademas internamente ordena la lista para lograr una mejor distribucion
  	*/
void distribuirBloquesEnNodos(t_list *bloquesEnArch, t_list *nodos);

	/*
  	* @NAME: eliminarArchivoPorNombre
  	* @DESC: recibe una lista de archivos y un nombre de un archivo, elimina al archivo de la lista devuelve el data eliminado o NULL si no lo encuentra
  	*/
void *eliminarArchivoPorNombre(char nombreBuscado[255],t_list *listaArchivos);

#endif /* FUNCIONESFILESYSTEM_H_ */
