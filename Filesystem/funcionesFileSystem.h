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
void eliminarArchivoPorNombre(char nombreBuscado[255],t_list *listaArchivos);

#endif /* FUNCIONESFILESYSTEM_H_ */
