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

	/*
	* @NAME: esNodoNuevo
	* @DESC: recibe una lista de nodos y un nodo, y se fija si ese nodo esta en la lista comparando puerto e ip
	*/
bool esNodoNuevo(t_nodo *nodoABuscar,t_list *listaNodos);


	/*
	* @NAME: activarNodoReconectado
	* @DESC: busca en la lista de nodos el nodo reconectado y le cambia el flag activandolo
	*/
void activarNodoReconectado(t_nodo *nodoABuscar,t_list *listaNodos);

	/*
	* @NAME: eliminarNodoYRerencias
	* @DESC: elimina nodo de la lista nodos y las referencias de las copias en los archivos si es que pertenecen a ese nodo
	*/
void eliminarNodoYRerencias(t_nodo *nodoAEliminar,t_list *listaNodos,t_list *archivos);


	/*
	* @NAME: eliminarReferencias
	* @DESC: //recorro una lista de archivos y por cada archivo recorro cada lista de bloques de archivo, por cada bloque de archivo veo si tengo que eliminar alguna copia de su lista de copias
	*/
void eliminarReferencias(t_nodo *nodoAEliminar,t_list *archivos);

/*
* @NAME: eliminarNodoDeLista
* @DESC: elimina nodo de la lista nodos
*/
void eliminarNodoDeLista(t_nodo *nodoAEliminar,t_list *listaNodos);

#endif /* FUNCIONESFILESYSTEM_H_ */
