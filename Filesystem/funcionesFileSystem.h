/*
 * funcionesFileSystem.h
 *
 *  Created on: 31/5/2015
 *      Author: JuanRios
 */

#ifndef FUNCIONESFILESYSTEM_H_
#define FUNCIONESFILESYSTEM_H_
#include"estructurasFileSystem.h"
/*
 * @NAME: distribuirBloquesEnNodos
 * @DESC: recibe una lista de bloques de archivo, por la cual crea 3 copias y cada una se la asigna a un nodo específico. Ademas internamente ordena la lista para lograr una mejor distribucion
 */
void distribuirBloquesEnNodos(t_list *bloquesEnArch, t_list *nodos);

/*
 * @NAME: formatear
 * @DESC: recibe la lista de archivos, nodos y directorios, y las vacía
 */
void formatear(t_list **listaNodos, t_list **listaArchivos,
		t_list **listaDirectorios);

/*
 * @NAME: renombrarArchivoPorNombre
 * @DESC: recibe una lista de archivos, un nombre de un archivo y un nuevo nombre para el mismo, y lo modifica en la lista de archivos
 */
void renombrarArchivoPorNombre(char *nombreBuscado, char *nuevoNombre,
		t_list *listaArchivos);

/*
 * @NAME: moverArchivoPorNombreYPadre
 * @DESC: recibe una lista de archivos, un nombre de un archivo y un nuevo padre para el mismo, y lo modifica en la lista de archivos
 */
void moverArchivoPorNombreYPadre(char *nombreBuscado, t_list *listaArchivos,
		t_list *listaDirectorios, int padre);

/*
 * @NAME: crearDirectorioDadoPadreYNom
 * @DESC: recibe una lista de directorios, un nombre de un directorio y un padre para el mismo, y lo crea agregando a lista de directorios
 */
void crearDirectorioDadoPadreYNom(char *nombre, int padre,
		t_list *listaDirectorio);

/*
 * @NAME: eliminarDirectorioDadoElIndice
 * @DESC: recibe una lista de directorios, un indice de un directorio y lo elimina
 */
void eliminarDirectorioDadoElIndice(int indice, t_list *listaDirectorio);

/*
 * @NAME: esNodoNuevo
 * @DESC: recibe una lista de nodos y un nodo, y se fija si ese nodo esta en la lista comparando puerto e ip
 */

bool esNodoNuevo(t_nodo *nodoABuscar, t_list *listaNodos);

/*
 * @NAME: activarNodoReconectado
 * @DESC: busca en la lista de nodos el nodo reconectado y le cambia el flag activandolo
 */
void activarNodoReconectado(t_nodo *nodoABuscar, t_list *listaNodos);

/*
 * @NAME: eliminarNodoYRerencias
 * @DESC: elimina nodo de la lista nodos y las referencias de las copias en los archivos si es que pertenecen a ese nodo
 */
void eliminarNodoYRerencias(t_nodo *nodoAEliminar, t_list *listaNodos,
		t_list *archivos);

/*
 * @NAME: eliminarReferencias
 * @DESC: //recorro una lista de archivos y por cada archivo recorro cada lista de bloques de archivo, por cada bloque de archivo veo si tengo que eliminar alguna copia de su lista de copias
 */
void eliminarReferencias(t_nodo *nodoAEliminar, t_list *archivos);

/*
 * @NAME: eliminarNodoDeLista
 * @DESC: elimina nodo de la lista nodos
 */
void eliminarNodoDeLista(t_nodo *nodoAEliminar, t_list *listaNodos);

//funciones de busquedas

/**
 * @NAME: buscarNodoPorIpPuerto
 * @DESC: busca un nodo por ipPuerto en una lista de nodos
 */
t_nodo *buscarNodoPorIpPuerto(char *ipPuerto, t_list *listaNodos);

/**
 * @NAME: buscarDirPorNombre
 * @DESC: busca un directorio por nombre en una lista de directorios
 */
t_directorio *buscarDirPorNombre(char *nombre, t_list *listaDirectorios);

/**
 * @NAME: buscarArchPorNombre
 * @DESC: busca un archivo por nombre en una lista de archivos
 */
t_archivo *buscarArchPorNombre(char *nombre, t_list *listaArchivos);

/**
 * @NAME: encontrarDirectorioHijo
 * @DESC: Devuelve el primer directorio de listaDirectorios que tenga como padre a directorioPadre
 */
t_directorio *encontrarDirectorioHijo(t_list *listaDirectorios,t_directorio *directorioPadre);

/**
 * @NAME: eliminarArchivoYreferencias
 * @DESC: elimina un archivo sacandolo de la lista y ademas en el nodo correspondiente elimina la copia que estaba y guarda en la cola de liberados el espacio liberado
 */
void eliminarArchivoYreferencias(t_archivo *unArchivo, t_list *listaArchivos,
		t_list *listaNodos);

/**
 * @NAME: eliminarArchivoDeLista
 * @DESC: elimina un archivo sacandolo de la lista
 */
void eliminarArchivoDeLista(t_archivo *unArchivo, t_list *listaArchivos);

/**
 * @NAME: renombrarDirectorioConNombre
 * @DESC: renombra un directorio con el nombre enviado por parametro
 */
void renombrarDirectorioConNombre(char *nombre,t_directorio *unDirectorio);

/**
 * @NAME: moverDirectorioConPadre
 * @DESC: le cambia el padre a un directorio, lo que provoca el movimiento del mismo
 */
void moverDirectorioConPadre(int padre,t_directorio *unDirectorio);
int obtenerArchivo(char *nombreArchivo, char* path, int directorioActual);

//135 lugar de trabajo de juanchi


t_directorio *buscarDirPorIndex(int index);
t_directorio *buscarDirPorPadre(int padre);
t_directorio *dameUnSubdir(t_directorio *unDirectorio);
int dirConSubdir(t_directorio *unDirectorio);
int dirVacio(t_directorio *unDirectorio) ;
t_archivo *buscarArchPorPadre(int padre);
t_archivo *dameUnSubArch(t_directorio *unDirectorio);
void eliminarDirectorioVacio(t_directorio *directorioAEliminar);
void eliminarSubArchivoDeDir(t_directorio *unDirectorio);







//148 fin de lugar de trabajo de juanchi

#endif /* FUNCIONESFILESYSTEM_H_ */
